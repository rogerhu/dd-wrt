#ifdef HAVE_AOSS
#define VALIDSOURCE 1

#ifdef WEBS
#include <webs.h>
#include <uemf.h>
#include <ej.h>
#else				/* !WEBS */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <httpd.h>
#include <errno.h>
#endif				/* WEBS */

#include <proto/ethernet.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/klog.h>
#include <sys/wait.h>
#include <cyutils.h>
#include <support.h>
#include <cy_conf.h>
// #ifdef EZC_SUPPORT
#include <ezc.h>
// #endif
#include <broadcom.h>
#include <wlutils.h>
#include <netdb.h>
#include <utils.h>
#include <stdarg.h>
#include <sha1.h>

struct variable **variables;

void aoss_save(webs_t wp)
{
	char buf[32];
	nvram_set("aoss_enable", websGetVar(wp, "aoss_enable", "0"));
	nvram_set("aoss_aes", websGetVar(wp, "aoss_aes", "0"));
	nvram_set("aoss_tkip", websGetVar(wp, "aoss_tkip", "0"));
	nvram_set("aoss_wep", websGetVar(wp, "aoss_wep", "0"));
#ifdef HAVE_WPS
	nvram_set("wps_enabled", websGetVar(wp, "wps_enabled", "0"));
	char *pin = websGetVar(wp, "pincode", NULL);
	if (pin)
		nvram_set("aoss_wep", pin);
#endif
	// check if at least one value was set
	if (nvram_match("aoss_aes", "0")
	    && nvram_match("aoss_tkip", "0")
	    && nvram_match("aoss_wep", "0")) {
		nvram_set("aoss_aes", "1");
	}
	if (strlen(nvram_safe_get("aoss_vifs"))) {
		nvram_unset("ath0_vifs");
		nvram_unset("aoss_vifs");
		nvram_commit();
	}
	if (strlen(nvram_safe_get("aossa_vifs"))) {
		nvram_unset("ath1_vifs");
		nvram_unset("aossa_vifs");
		nvram_commit();
	}
	// all other vars
	//validate_cgi(wp);
}

int aoss_status(void)
{
	if (pidof("aoss") > 0) {
		return 1;
	}
	return 0;
}

void aoss_start(webs_t wp)
{
	fprintf(stderr, "[AOSS] start\n");
	if (!aoss_status()) {
		system("startservice_f aoss");
		fprintf(stderr, "[AOSS] start\n");
	}
	return;
}
#endif
