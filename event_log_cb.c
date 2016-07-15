#include <event2/event.h>
#include <stdio.h>

static void discard_cb(int severity, const char *msg)
{
	/* This callback does nothing */
	severity = 0;
	msg = NULL;
}

static FILE *logfile = NULL;
static void write_to_file_cb(int serverity, const char *msg)
{
	const char *s;
	if (!logfile)
		return;
	switch (serverity)
	{
		case _EVENT_LOG_DEBUG:
			s = "DEBUG";
			break;
		case _EVENT_LOG_MSG:
			s = "MSG";
			break;
		case _EVENT_LOG_WARN:
			s = "WARN";
			break;
		case _EVENT_LOG_ERR:
			s = "ERROR";
			break;
		default:
			s = "UNKNOWN";
			break;
	}

	fprintf(logfile, "[%s] %s\n", s, msg);
}

/* Turn off all logging from Libevent. */
void suppress_logging(void)
{
	event_set_log_callback(discard_cb);
}

/* Redirect all Libevent log message to the C stdio file 'f'. */
void set_logfile(FILE *f)
{
	logfile = f;
	event_set_log_callback(write_to_file_cb);
}
