#ifndef STAR6E_CUS3A_H
#define STAR6E_CUS3A_H

#include <stdint.h>

/** Custom AE configuration.
 *  Gain/shutter limits are seeded from the ISP bin at startup;
 *  target Y and step size are compile-time constants. */
typedef struct {
	uint32_t sensor_fps;       /* sensor output fps (for max shutter calc) */
	uint32_t ae_fps;           /* AE processing rate in Hz (default 15) */
	int      verbose;          /* enable periodic status logging */
} Star6eCus3aConfig;

/** Fill config with sensible FPV defaults. */
void star6e_cus3a_config_defaults(Star6eCus3aConfig *cfg);

/**
 * Start the custom AE thread.
 *
 * Pauses the ISP's internal AE and runs a simple proportional
 * controller at the configured rate.  Call after ISP bin is loaded
 * and CUS3A has been enabled (1,1,1).
 *
 * Returns 0 on success, -1 on error.
 */
int star6e_cus3a_start(const Star6eCus3aConfig *cfg);

/**
 * Stop the custom AE thread and resume ISP internal AE.
 * Safe to call if the thread was never started.
 */
void star6e_cus3a_stop(void);

/** Signal the thread to stop (non-blocking).  Call join() later. */
void star6e_cus3a_request_stop(void);

/** Wait for the thread to exit after request_stop(). */
void star6e_cus3a_join(void);

/** Return 1 if the custom AE thread is running. */
int star6e_cus3a_running(void);

/** Update the max shutter time (us) at runtime.
 *  Called by the exposure control when the user changes isp.exposure. */
void star6e_cus3a_set_shutter_max(uint32_t max_us);

/** Set manual AWB override.  When manual=1, the custom AWB loop is
 *  paused and the caller controls white balance directly.
 *  When manual=0, the grey-world AWB loop resumes. */
void star6e_cus3a_set_awb_manual(int manual);

#endif /* STAR6E_CUS3A_H */
