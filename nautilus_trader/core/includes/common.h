/* Generated with cbindgen:0.24.3 */

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdint.h>
#include <Python.h>

typedef enum LogColor {
    NORMAL = 0,
    GREEN = 1,
    BLUE = 2,
    MAGENTA = 3,
    CYAN = 4,
    YELLOW = 5,
    RED = 6,
} LogColor;

typedef enum LogLevel {
    DEBUG = 10,
    INFO = 20,
    WARNING = 30,
    ERROR = 40,
    CRITICAL = 50,
} LogLevel;

typedef enum MessageCategory {
    Command,
    Document,
    Event,
    Request,
    Response,
} MessageCategory;

typedef struct Logger_t Logger_t;

typedef struct String String;

typedef struct TestClock TestClock;

typedef struct CTestClock {
    struct TestClock *_0;
} CTestClock;

/**
 * Represents a time event occurring at the event timestamp.
 */
typedef struct TimeEvent_t {
    /**
     * The event name.
     */
    struct String *name;
    /**
     * The event ID.
     */
    enum MessageCategory category;
    /**
     * The UNIX timestamp (nanoseconds) when the time event occurred.
     */
    UUID4_t event_id;
    /**
     * The message category
     */
    uint64_t ts_event;
    /**
     * The UNIX timestamp (nanoseconds) when the object was initialized.
     */
    uint64_t ts_init;
} TimeEvent_t;

typedef struct Vec_TimeEvent {
    const struct TimeEvent_t *ptr;
    uintptr_t len;
} Vec_TimeEvent;

/**
 * Logger is not C FFI safe, so we box and pass it as an opaque pointer.
 * This works because Logger fields don't need to be accessed, only functions
 * are called.
 */
typedef struct CLogger {
    struct Logger_t *_0;
} CLogger;

struct CTestClock test_clock_new(void);

void test_clock_free(struct CTestClock clock);

void test_clock_set_time(struct CTestClock *clock, uint64_t to_time_ns);

uint64_t test_clock_time_ns(const struct CTestClock *clock);

PyObject *test_clock_timer_names(const struct CTestClock *clock);

uintptr_t test_clock_timer_count(struct CTestClock *clock);

/**
 * # Safety
 * - `name` must be borrowed from a valid Python UTF-8 `str`.
 */
void test_clock_set_time_alert_ns(struct CTestClock *clock, PyObject *name, uint64_t alert_time_ns);

/**
 * # Safety
 * - `name` must be borrowed from a valid Python UTF-8 `str`.
 */
void test_clock_set_timer_ns(struct CTestClock *clock,
                             PyObject *name,
                             uint64_t interval_ns,
                             uint64_t start_time_ns,
                             uint64_t stop_time_ns);

struct Vec_TimeEvent test_clock_advance_time(struct CTestClock *clock, uint64_t to_time_ns);

void vec_time_events_drop(struct Vec_TimeEvent v);

/**
 * # Safety
 * - `name` must be borrowed from a valid Python UTF-8 `str`.
 */
uint64_t test_clock_next_time_ns(struct CTestClock *clock, PyObject *name);

/**
 * # Safety
 * - `name` must be borrowed from a valid Python UTF-8 `str`.
 */
void test_clock_cancel_timer(struct CTestClock *clock, PyObject *name);

void test_clock_cancel_timers(struct CTestClock *clock);

/**
 * Creates a logger from a valid Python object pointer and a defined logging level.
 *
 * # Safety
 * - `trader_id_ptr` must be borrowed from a valid Python UTF-8 `str`.
 * - `machine_id_ptr` must be borrowed from a valid Python UTF-8 `str`.
 * - `instance_id_ptr` must be borrowed from a valid Python UTF-8 `str`.
 */
struct CLogger logger_new(PyObject *trader_id_ptr,
                          PyObject *machine_id_ptr,
                          PyObject *instance_id_ptr,
                          enum LogLevel level_stdout,
                          uint8_t is_bypassed);

void logger_free(struct CLogger logger);

void flush(struct CLogger *logger);

/**
 * Return the loggers trader ID.
 *
 * # Safety
 * - Assumes that since the data is originating from Rust, the GIL does not need
 * to be acquired.
 * - Assumes you are immediately returning this pointer to Python.
 */
PyObject *logger_get_trader_id(const struct CLogger *logger);

/**
 * Return the loggers machine ID.
 *
 * # Safety
 * - Assumes that since the data is originating from Rust, the GIL does not need
 * to be acquired.
 * - Assumes you are immediately returning this pointer to Python.
 */
PyObject *logger_get_machine_id(const struct CLogger *logger);

UUID4_t logger_get_instance_id(const struct CLogger *logger);

uint8_t logger_is_bypassed(const struct CLogger *logger);

/**
 * Log a message from valid Python object pointers.
 *
 * # Safety
 * - `component_ptr` must be borrowed from a valid Python UTF-8 `str`.
 * - `msg_ptr` must be borrowed from a valid Python UTF-8 `str`.
 */
void logger_log(struct CLogger *logger,
                uint64_t timestamp_ns,
                enum LogLevel level,
                enum LogColor color,
                PyObject *component_ptr,
                PyObject *msg_ptr);

void time_event_free(struct TimeEvent_t event);

/**
 * # Safety
 * - `name` must be borrowed from a valid Python UTF-8 `str`.
 */
struct TimeEvent_t time_event_new(PyObject *name,
                                  UUID4_t event_id,
                                  uint64_t ts_event,
                                  uint64_t ts_init);

/**
 * Returns a pointer to a valid Python UTF-8 string.
 *
 * # Safety
 * - Assumes that since the data is originating from Rust, the GIL does not need
 * to be acquired.
 * - Assumes you are immediately returning this pointer to Python.
 */
PyObject *time_event_name(const struct TimeEvent_t *event);
