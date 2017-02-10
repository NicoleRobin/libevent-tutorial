struct event_base;
event_base_new()
event_base_free()
event_base_loop()
event_base_new_with_config()

struct event;
event_new()
event_free()
event_assign()
event_get_assignment()
event_add()
event_del()
event_active()
event_pending()
event_get_fd()
event_get_base()
event_get_events()
event_get_callback()
event_get_callback_arg()
event_priority_set()

struct event_config;
event_config_new()
event_config_free()
event_base_new_with_config()
event_config_avoid_method()
event_config_require_features()
event_config_set_flag()
event_config_set_num_cpus_hint()

event_enable_debug_mode()
event_debug_unassign()

event_reinit()

event_base_dispatch()

event_get_method()
event_get_support_methods()

event_gettime_monotonic()

int event_base_get_num_events(struct event_base *base, unsigned int flags)
flags:
#define EVENT_BASE_COUNT_ACTIVE
#define EVENT_BASE_COUNT_VIRTUAL
#define EVENT_BASE_COUNT_ADDED

int event_get_max_events(struct event_base *base, unsigned int flags, int)

struct event_config* event_config_new()
void event_config_free(struct event_config *cfg)

int event_config_avoid_method(struct event_config *cfg, const char *method);

enum event_method_feature
{
	EV_FEATURE_ET = 0x01,
	EV_FEATURE_O1 = 0x02,
	EV_FEATURE_FDS = 0x04,
	EV_FEATURE_EARLY_CLOSE = 0x08
};

enum event_base_config_flag
{
	EVENT_BASE_FLAG_NOLOCK = 0x01,
	EVENT_BASE_FLAG_IGNORE_ENV = 0x02,
	EVENT_BASE_FLAG_STARTUP_IOCP = 0x04,
	EVENT_BASE_FLAG_NO_CACHE_TIME = 0x08,
	EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST = 0x10,
	EVENT_BASE_FLAG_PERCISE_TIMER = 0x20
};

int event_base_get_features(const struct event_base *base)
int event_config_require_features(struct event_config *cfg, int feature)
int event_config_set_flag(struct event_config *cfg, int flag)
int event_config_set_num_cpus_hint(struct event_config *cfg, int cpus)
int event_config_set_max_dispatch_interval(struct event_config *cfg, const struct timeval *max_interval, int max_callbacks, int min_priority)

struct event_base* event_base_new_with_config(const struct event_config *cfg)
void event_base_free(struct event_base *base)
void event_base_free_nofinalize(struct event_base *base)

#define EVENT_LOG_DEBUG
#define EVENT_LOG_MSG
#define EVENT_LOG_WARN
#define EVENT_LOG_ERR

typedef void (*event_log_cb)(int serverity, const char *msg)
void event_set_log_callback(event_log_cb cb)

typedef void (*event_fatal_cb)(int err)
void event_set_fatal_callback(event_fatal_cb cb)

#define EVENT_DBG_ALL 0xffffffffu
#define EVENT_DBG_NONE 0

void event_enable_debug_logging(ev_uint32_t which)

int event_base_set(struct event_base *base, struct event *event)

#define EVLOOP_ONCE 0x01
#define EVLOOP_NONBLOCK 0x02
#define EVLOOP_NO_EXIT_EMPTY 0x04
int event_base_loop(struct event_base *base, int flag)

int event_base_loopexit(struct event_base *base, const struct timeval *tv)
int event_base_loopbreak(struct event_base *base)
int event_base_loopcontinue(struct event_base *base)

int event_base_got_exit(struct event_base *base)
int event_base_got_break(struct event_base *base)

#define EV_TIMEOUT
#define EV_READ
#define EV_WRITE
#define EV_SIGNAL
#define EV_PERSIST
#define EV_ET
#define EV_FINALIZE
#define EV_CLOSE

#define evtimer_assign(ev, b, cb, arg) event_assign((ev), (b), -1, 0, (cb), (arg))
#define evtimer_new(b, cb, arg) event_new((b), -1, 0, (cb), (arg))
#define evtimer_add(ev, tv) event_add((ev), (tv))
#define evtimer_del(ev) event_del(ev)
#define evtimer_pending(ev, tv) event_pending((ev), EV_TIMEOUT, (tv))
#define evtimer_initialized(ev) event_initialized(ev)

#define evsignal_add(ev, tv) event_add((ev), (tv))
#define evsignal_assign(ev, b, x, cb, arg) event_assign((ev), (b), (x), EV_SIGNAL | EV_PERSIST, cb, (arg))
#define evsignal_new(b, x, cb, arg) event_new((b), (x), EV_SIGNAL | EV_PERSIST, (cb), (arg))
#defing evsignal_pending(ev, tv) event_pending((ev), EV_SIGNAL, (tv))
#defing evsignal_initialized(ev) event_initialized(ev)

typedef void (*event_callback_fn)(evutil_socket_t fd, short flags, void *arg)

struct event* event_new(struct event_base *base, evutil_socket_t fd, short events, event_callback_fn fn, void *arg)
int event_assign(struct event *event, struct event_base *base, evutil_socket_t fd, short events, event_callback_fn fn, void *arg)
void event_free(struct event *event)

typedef void (*event_finalize_callback_fn)(struct event *event, void *arg)
int event_finalize(unsigned, struct event *event, event_finalize_callback_fn fn)
int event_free_finalize(unsigned, struct event *event, event_finalize_callback_fn fn)

int event_base_once(struct event_base *base, evutil_socket_t fd, short , event_callback_fn, void *, const struct timeval)

int event_add(struct event *ev, const struct timeval *timeout)
int event_remove_timer(struct event *ev)
int event_del(struct event *event)
int event_del_noblock(struct event *ev)
int event_del_block(struct event *ev)
int event_active(struct event *ev, int res, short ncalls)
int event_pending(const struct event *event, short events, struct timeval *tv)
struct event* event_base_get_running_event(struct event_base *base)
int event_initialized(const struct event *ev)

#define event_get_signal(ev) ((int)event_get_fd(ev))
evutil_socket_t event_get_fd(const struct event *ev)

struct event_base* event_get_base(const struct event *ev)
short event_get_events(const struct event *ev)
event_callback_fn eveng_get_callback(const struct event *ev)
void* event_get_callback_arg(const struct event *ev)
int event_get_priority(const struct event *ev)

void eveng_get_assignment(const struct event *event, struct event_base **base_out, evutil_socket_t *fd_out, 
short *event_out, event_callback_fn *callback_out, void **arg_out)

size_t event_get_struct_event_size(void);

const char* event_get_version(void)

ev_uint32_t event_get_version_number(void)

int event_base_priority_init(struct event_base *base, int)
int event_base_get_npriorities(struct event_base *eb)

int event_priority_set(strut event *event, int)
void event_set_mem_functions()

void event_base_dump_events(struct event_base *base, FILE *pf)
void event_base_active_by_fd(struct event_base *base, evutil_socket_t fd, short events)
void event_base_active_by_signal(struct event_base *base, int sig)

typedef int (*event_base_foreach_event_cb)(const struct event_base *base, const struct event *event, void *arg)
int event_base_foreach_event(struct event_base *base, event_base_foreach_event_cb fn, void *arg)

int event_base_gettimeofday_cached(struct event_base *base, struct timeval *tv)
int event_base_update_cache_time(struct event_base *base)

void libevent_global_shutdown(void)