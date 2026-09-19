*This project has been created as part of the 42 curriculum by josamba-.*

# Codexion

## Description

Codexion is a concurrency simulation written in C with POSIX threads. Coders sit
around a circular co-working hub and share a limited number of USB dongles. A
coder needs **two dongles held simultaneously** — the one on their left and the
one on their right — in order to compile. Once compiling is over, both dongles
go back on the table and the coder moves on to debugging, then refactoring, then
tries to compile again.

A coder who does not start compiling within `time_to_burnout` milliseconds of
their last compile burns out, and the simulation stops. The simulation also
stops once every coder has compiled at least `number_of_compiles_required`
times.

The interesting part is not the simulation itself but the arbitration: when
several coders want the same dongle, who gets it? Codexion implements two
scheduling policies backed by a hand-written binary min-heap priority queue, one
per dongle:

- **FIFO** — the dongle is granted to whoever asked first.
- **EDF** (Earliest Deadline First) — the dongle is granted to the coder whose
  burnout deadline (`last_compile_start + time_to_burnout`) is nearest.

The whole point of EDF here is **liveness**: as long as the parameters are
feasible, no coder should ever starve and burn out.

Each coder is a thread. A separate monitor thread watches for burnout and for
the completion condition. There are no global variables; all shared state lives
in a single `t_config` structure passed by pointer.

## Instructions

### Build

```bash
make          # build ./codexion
make clean    # remove object files
make fclean   # remove object files and the binary
make re       # fclean + all
```

The project compiles with `cc -Wall -Wextra -Werror` and links with `-pthread`.
No external library is required.

### Run

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All eight arguments are mandatory.

| Argument | Meaning |
|---|---|
| `number_of_coders` | Number of coders, and also the number of dongles. Must be > 0. |
| `time_to_burnout` | Milliseconds. A coder burns out if this much time passes without starting a compile. |
| `time_to_compile` | Milliseconds spent compiling, holding two dongles. |
| `time_to_debug` | Milliseconds spent debugging. |
| `time_to_refactor` | Milliseconds spent refactoring, after which the coder immediately tries to compile again. |
| `number_of_compiles_required` | The simulation stops once every coder has compiled at least this many times. |
| `dongle_cooldown` | Milliseconds during which a released dongle stays unavailable. |
| `scheduler` | Exactly `fifo` or `edf`. |

Invalid input is rejected with a usage message and exit code 1: wrong argument
count, non-integer values, negative numbers, zero coders, values above
`INT_MAX`, or a scheduler other than `fifo`/`edf`.

### Examples

```bash
# Feasible parameters, EDF — every coder compiles 3 times, nobody burns out
./codexion 5 800 200 200 200 3 0 edf

# Infeasible parameters — a coder burns out around t = 310 ms
./codexion 4 310 200 100 0 999 0 fifo

# A single coder: one dongle on the table, so compiling is impossible
./codexion 1 200 100 100 100 5 0 fifo
```

### Output format

```
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

Timestamps are milliseconds since the start of the simulation; `X` is the coder
number, from 1 to `number_of_coders`. Two messages are never interleaved on a
single line, and no state message is ever printed after a burnout announcement.

## Blocking cases handled

### Deadlock prevention — breaking circular wait

Of Coffman's four conditions, three are inherent to the problem and are kept:
a dongle is exclusive (**mutual exclusion**), a coder holds their first dongle
while waiting for the second (**hold and wait**), and a dongle is never taken
away from a coder by force (**no preemption**).

The fourth condition, **circular wait**, is the one that is broken, using a
global resource ordering. At initialization, `assign_dongle` compares the ids of
the two dongles adjacent to a coder and stores them so that `first_dongle`
always holds the lower id. Since `take_dongles` acquires `first_dongle` before
`second_dongle`, every coder in the hub requests dongles in ascending id order.
No cycle can form in the wait-for graph, so no deadlock is possible — including
in the classic failure case where all coders grab their left dongle at once.

Dongles are released in the reverse order (`second`, then `first`), and if a
coder acquires the first dongle but fails to acquire the second, the first one
is released immediately rather than held.

### Starvation prevention — priority queue arbitration

A dongle is never granted on a first-come-first-woken basis, because that would
let an unlucky coder be passed over indefinitely. Each dongle owns its own
**binary min-heap** (`t_heap`, implemented from scratch in `heap.c` /
`heap_utils.c`; no standard-library structure is used).

When a coder wants a dongle it pushes a request into that dongle's heap, keyed
by the active policy:

- FIFO uses a per-dongle monotonically increasing ticket counter.
- EDF uses `last_compile + time_to_burnout`, read under the coder's mutex.

A waiting coder may only take the dongle when it is free, its cooldown has
elapsed, **and it is the request at the top of the heap**. Equal keys are
broken deterministically by the lower coder id, so the EDF policy is fully
ordered even when two deadlines land on the same millisecond.

Under EDF this yields the liveness guarantee required by the subject: the coder
closest to burning out is always served first.

### Cooldown handling

When a dongle is released, `release_one_dongle` records
`free_at = now + dongle_cooldown` and broadcasts on the dongle's condition
variable. A waiter that finds the dongle free but still cooling down does not
spin: it calls `pthread_cond_timedwait` with `free_at` converted to an absolute
`struct timespec`, so it sleeps exactly until the cooldown expires and no
longer. When the dongle is genuinely busy, the waiter blocks on
`pthread_cond_wait` instead and is woken by the next release.

### Precise burnout detection

Burnout is detected by a dedicated monitor thread, never by the coders
themselves. The monitor loops every millisecond (`usleep(1000)`), reading each
coder's `last_compile` under that coder's own mutex and comparing it against
`time_to_burnout`. A 1 ms polling period keeps the announcement well inside the
10 ms window required by the subject — measured latency is 1 to 2 ms.

On burnout the monitor sets `config->end` under `state_lock`, broadcasts on
every dongle's condition variable so that no coder stays blocked in
`pthread_cond_wait`, and only then prints the burnout line. This guarantees all
threads terminate and that every `pthread_join` returns.

### Log serialization

All output goes through `log_action`, which takes `print_lock` **before**
checking whether the simulation is over. That ordering matters: once
`stop_simulation` has set the end flag, no coder can print a state message, even
one that had already decided to print. The burnout line is therefore always the
last line of output, and because `printf` is called under the lock, two messages
can never interleave on a single line.

### Memory management

Every heap allocation is owned by `t_config` and lives for the whole run: the
dongle array, the coder array, and one request buffer per dongle heap. Ownership
is central, so release is central too — `clean_config` in `free.c` is the single
teardown path, called on both the success and the failure route out of `main`.

Partial initialization is handled explicitly. `init_heap` is called first in the
per-dongle loop, before the mutex and condition variable are created, so if it
fails at dongle `i` then exactly dongles `0..i-1` are fully built.
`fail_dongles` destroys that many, frees the array and sets the pointer to
`NULL`, which keeps `clean_config` from touching it a second time. `t_config` is
zeroed with `memset` before parsing, so no pointer is ever read uninitialized.

Mutexes and condition variables are destroyed before the array containing them
is freed, and only after every thread has been joined, so no
`pthread_mutex_destroy` can fail with `EBUSY`.

Verified with `valgrind --leak-check=full --show-leak-kinds=all`: all heap
blocks freed, zero errors. Verified with `valgrind --tool=helgrind`: zero data
races and zero lock-order inversions.

## Thread synchronization mechanisms

### Threads

- One thread per coder, created with `pthread_create`, running `coder_routine`.
- One monitor thread running `monitor_routine`.
- `main` joins the monitor first, then every coder thread, and only then tears
  down the shared state.

### Primitives and what each one protects

| Primitive | Location | Protects |
|---|---|---|
| `pthread_mutex_t mutex` | per dongle | `available`, `free_at`, `ticket`, and the dongle's waiting heap |
| `pthread_cond_t cond` | per dongle | wakes waiters when the dongle is released or when the simulation ends |
| `pthread_mutex_t mutex` | per coder | `last_compile` and `compile_count` |
| `pthread_mutex_t print_lock` | global (in `t_config`) | serializes all output |
| `pthread_mutex_t state_lock` | global (in `t_config`) | the `end` flag |

Giving each dongle and each coder its own mutex — rather than one big lock —
means that coders working on opposite sides of the hub never contend with each
other.

### How race conditions are prevented

**Dongle state.** `available`, `free_at` and the waiting heap are only ever read
or written with the dongle's mutex held. The acquisition path in
`take_one_dongle` locks the mutex, pushes the request, and then waits inside
`not_available_path` using `pthread_cond_wait` / `pthread_cond_timedwait`, which
atomically release the mutex while sleeping and re-acquire it on wake-up. The
wait is a `while` loop, not an `if`, so a spurious wake-up or a broadcast
intended for another coder simply re-evaluates the condition instead of
proceeding on a stale assumption.

**Coder counters.** `last_compile` and `compile_count` are written by the coder
thread in `update_compile` and read by the monitor in `check_burnout` and
`finished`, and by the EDF key function. Every one of those accesses takes the
coder's mutex, and the monitor copies the value into a local before releasing,
so it never reasons about a field that another thread may be modifying.

**Simulation state.** `end` is written once by `stop_simulation` and read by
every coder through `sim_is_over`, both under `state_lock`.

### Thread-safe communication between coders and monitor

Coders and the monitor never call each other. They communicate through two
pieces of guarded shared state:

- *coder to monitor*: each coder publishes its progress by updating
  `last_compile` and `compile_count` under its own mutex. The monitor polls
  those fields.
- *monitor to coders*: the monitor raises `end` under `state_lock`, then calls
  `wake_all_dongles`, which takes each dongle's mutex in turn and broadcasts on
  its condition variable. A coder blocked in `pthread_cond_wait` wakes, sees
  `sim_is_over`, releases the dongle mutex and returns from its routine.

The broadcast is what makes shutdown reliable: without it a coder waiting on a
dongle that nobody will ever release would block forever and `pthread_join`
would never return.

### A note on lock ordering

`log_action` holds `print_lock` and then acquires `state_lock` through
`sim_is_over`. `stop_simulation` uses the two locks in the opposite order but
never nests them — `state_lock` is released before `print_lock` is taken. That
is what keeps the pair free of an ABBA deadlock, and it is a constraint to
preserve when modifying either function.

## Resources

### Documentation and references

- `man` pages: `pthread_create`, `pthread_join`, `pthread_mutex_init`,
  `pthread_cond_wait`, `pthread_cond_timedwait`, `pthread_cond_broadcast`,
  `gettimeofday`, `usleep`
- The Open Group Base Specifications, POSIX threads
- Dijkstra's Dining Philosophers problem, and the resource-hierarchy solution
  to circular wait
- Coffman, Elphick, Shoshani — *System Deadlocks* (1971), for the four
  conditions of deadlock
- Liu & Layland — *Scheduling Algorithms for Multiprogramming in a Hard
  Real-Time Environment* (1973), for Earliest Deadline First
- Valgrind documentation, in particular the Memcheck and Helgrind tools

### Use of AI

AI was used as a tutor, not as a code generator. Every line of source in this
repository was written by hand.

Its role was to clarify concepts when the available resources — man pages,
specifications, articles — were unclear or assumed knowledge I did not yet have.
When a piece of documentation left a question open, I used AI to rephrase it,
work through an example, or confirm that I had understood it correctly, and
then went back to the primary source to verify the answer.

It was also used for tooling guidance — in particular which Valgrind
invocations are relevant to a multithreaded project, and what the flags
actually do: `--leak-check=full`, `--show-leak-kinds=all` and
`--track-origins=yes` for Memcheck, and when to reach for `--tool=helgrind` or
`--tool=drd` instead to look for data races rather than leaks.