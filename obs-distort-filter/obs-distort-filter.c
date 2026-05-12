#include <obs-module.h>
#include <graphics/vec2.h>
#include <graphics/vec4.h>
#include <util/platform.h>

#define SETTING_PIXEL_SIZE       "pixel_size"
#define SETTING_RGB_SHIFT        "rgb_shift"
#define SETTING_WAVE_AMOUNT      "wave_amount"
#define SETTING_WAVE_SPEED       "wave_speed"
#define SETTING_SCANLINE_INT     "scanline_intensity"
#define SETTING_GLITCH_INTENSITY "glitch_intensity"
#define SETTING_PACKET_LOSS      "packet_loss"
#define SETTING_FREEZE_FRAMES    "freeze_frames"
#define SETTING_FREEZE_DURATION  "freeze_duration"
#define SETTING_RANDOMIZE_PIXEL  "randomize_pixel"
#define SETTING_RANDOMIZE_WAVE   "randomize_wave"
#define SETTING_RANDOMIZE_GLITCH "randomize_glitch"
#define SETTING_INTERLACE        "interlace"
#define SETTING_FLICKER          "flicker"
#define SETTING_GHOST            "ghost"
#define SETTING_HORIZONTAL_HOLD  "horizontal_hold"
#define SETTING_POSTERIZE        "posterize"
#define SETTING_ROLLING_BAR      "rolling_bar"
#define SETTING_CHROMA_BLEED     "chroma_bleed"

#define TEXT_PIXEL_SIZE          obs_module_text("Distort.PixelSize")
#define TEXT_RGB_SHIFT           obs_module_text("Distort.RGBShift")
#define TEXT_WAVE_AMOUNT         obs_module_text("Distort.WaveAmount")
#define TEXT_WAVE_SPEED          obs_module_text("Distort.WaveSpeed")
#define TEXT_SCANLINE_INT        obs_module_text("Distort.ScanlineIntensity")
#define TEXT_GLITCH_INTENSITY    obs_module_text("Distort.GlitchIntensity")
#define TEXT_PACKET_LOSS         obs_module_text("Distort.PacketLoss")
#define TEXT_FREEZE_FRAMES       obs_module_text("Distort.FreezeFrames")
#define TEXT_FREEZE_DURATION     obs_module_text("Distort.FreezeDuration")
#define TEXT_RANDOMIZE_PIXEL     obs_module_text("Distort.RandomizePixel")
#define TEXT_RANDOMIZE_WAVE      obs_module_text("Distort.RandomizeWave")
#define TEXT_RANDOMIZE_GLITCH    obs_module_text("Distort.RandomizeGlitch")
#define TEXT_INTERLACE           obs_module_text("Distort.Interlace")
#define TEXT_FLICKER             obs_module_text("Distort.Flicker")
#define TEXT_GHOST               obs_module_text("Distort.Ghost")
#define TEXT_HORIZONTAL_HOLD     obs_module_text("Distort.HorizontalHold")
#define TEXT_POSTERIZE           obs_module_text("Distort.Posterize")
#define TEXT_ROLLING_BAR         obs_module_text("Distort.RollingBar")
#define TEXT_CHROMA_BLEED        obs_module_text("Distort.ChromaBleed")

struct distort_filter_data {
	obs_source_t *context;

	gs_effect_t *effect;
	gs_eparam_t *param_pixel_size;
	gs_eparam_t *param_rgb_shift;
	gs_eparam_t *param_wave_amount;
	gs_eparam_t *param_wave_speed;
	gs_eparam_t *param_scanline_int;
	gs_eparam_t *param_glitch_int;
	gs_eparam_t *param_time;
	gs_eparam_t *param_rand_seed;
	gs_eparam_t *param_uv_size;
	gs_eparam_t *param_packet_loss;
	gs_eparam_t *param_freeze_time;
	gs_eparam_t *param_interlace;
	gs_eparam_t *param_flicker;
	gs_eparam_t *param_ghost;
	gs_eparam_t *param_horizontal_hold;
	gs_eparam_t *param_posterize;
	gs_eparam_t *param_rolling_bar;
	gs_eparam_t *param_chroma_bleed;

	float pixel_size;
	float rgb_shift;
	float wave_amount;
	float wave_speed;
	float scanline_int;
	float glitch_int;
	float packet_loss;
	float freeze_frames;
	float freeze_duration;
	float randomize_pixel;
	float randomize_wave;
	float randomize_glitch;
	float interlace;
	float flicker;
	float ghost;
	float horizontal_hold;
	float posterize;
	float rolling_bar;
	float chroma_bleed;

	float time;
	float rand_seed;
	float freeze_time_value;
	float freeze_seed_value;
	float freeze_timer;
	float next_freeze;
	bool freeze_active;
};

static inline float random_val(struct distort_filter_data *filter, float offset)
{
	float val = sinf(filter->rand_seed * 127.1f + filter->time * 37.3f + offset * 17.7f) * 43758.5453f;
	return val - floorf(val);
}

static const char *distort_filter_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("DistortFilter");
}

static void *distort_filter_create(obs_data_t *settings, obs_source_t *context)
{
	struct distort_filter_data *filter = bzalloc(sizeof(*filter));
	char *effect_path = obs_module_file("distort_filter.effect");

	filter->context = context;

	obs_enter_graphics();
	filter->effect = gs_effect_create_from_file(effect_path, NULL);
	obs_leave_graphics();

	bfree(effect_path);

	if (!filter->effect) {
		bfree(filter);
		return NULL;
	}

	filter->param_pixel_size     = gs_effect_get_param_by_name(filter->effect, "pixel_size");
	filter->param_rgb_shift      = gs_effect_get_param_by_name(filter->effect, "rgb_shift");
	filter->param_wave_amount    = gs_effect_get_param_by_name(filter->effect, "wave_amount");
	filter->param_wave_speed     = gs_effect_get_param_by_name(filter->effect, "wave_speed");
	filter->param_scanline_int   = gs_effect_get_param_by_name(filter->effect, "scanline_intensity");
	filter->param_glitch_int     = gs_effect_get_param_by_name(filter->effect, "glitch_intensity");
	filter->param_time           = gs_effect_get_param_by_name(filter->effect, "time");
	filter->param_rand_seed      = gs_effect_get_param_by_name(filter->effect, "rand_seed");
	filter->param_uv_size        = gs_effect_get_param_by_name(filter->effect, "uv_size");
	filter->param_packet_loss    = gs_effect_get_param_by_name(filter->effect, "packet_loss");
	filter->param_freeze_time    = gs_effect_get_param_by_name(filter->effect, "freeze_time");
	filter->param_interlace      = gs_effect_get_param_by_name(filter->effect, "interlace");
	filter->param_flicker        = gs_effect_get_param_by_name(filter->effect, "flicker");
	filter->param_ghost          = gs_effect_get_param_by_name(filter->effect, "ghost");
	filter->param_horizontal_hold = gs_effect_get_param_by_name(filter->effect, "horizontal_hold");
	filter->param_posterize      = gs_effect_get_param_by_name(filter->effect, "posterize");
	filter->param_rolling_bar    = gs_effect_get_param_by_name(filter->effect, "rolling_bar");
	filter->param_chroma_bleed   = gs_effect_get_param_by_name(filter->effect, "chroma_bleed");

	filter->next_freeze = 1.0f;

	obs_source_update(context, settings);
	return filter;
}

static void distort_filter_destroy(void *data)
{
	struct distort_filter_data *filter = data;

	obs_enter_graphics();
	gs_effect_destroy(filter->effect);
	obs_leave_graphics();

	bfree(filter);
}

static void distort_filter_update(void *data, obs_data_t *settings)
{
	struct distort_filter_data *filter = data;

	filter->pixel_size       = (float)obs_data_get_double(settings, SETTING_PIXEL_SIZE);
	filter->rgb_shift        = (float)obs_data_get_double(settings, SETTING_RGB_SHIFT);
	filter->wave_amount      = (float)obs_data_get_double(settings, SETTING_WAVE_AMOUNT);
	filter->wave_speed       = (float)obs_data_get_double(settings, SETTING_WAVE_SPEED);
	filter->scanline_int     = (float)obs_data_get_double(settings, SETTING_SCANLINE_INT);
	filter->glitch_int       = (float)obs_data_get_double(settings, SETTING_GLITCH_INTENSITY);
	filter->packet_loss      = (float)obs_data_get_double(settings, SETTING_PACKET_LOSS);
	filter->freeze_frames    = (float)obs_data_get_double(settings, SETTING_FREEZE_FRAMES);
	filter->freeze_duration  = (float)obs_data_get_double(settings, SETTING_FREEZE_DURATION);
	filter->randomize_pixel  = (float)obs_data_get_double(settings, SETTING_RANDOMIZE_PIXEL);
	filter->randomize_wave   = (float)obs_data_get_double(settings, SETTING_RANDOMIZE_WAVE);
	filter->randomize_glitch = (float)obs_data_get_double(settings, SETTING_RANDOMIZE_GLITCH);
	filter->interlace        = (float)obs_data_get_double(settings, SETTING_INTERLACE);
	filter->flicker          = (float)obs_data_get_double(settings, SETTING_FLICKER);
	filter->ghost            = (float)obs_data_get_double(settings, SETTING_GHOST);
	filter->horizontal_hold  = (float)obs_data_get_double(settings, SETTING_HORIZONTAL_HOLD);
	filter->posterize        = (float)obs_data_get_double(settings, SETTING_POSTERIZE);
	filter->rolling_bar      = (float)obs_data_get_double(settings, SETTING_ROLLING_BAR);
	filter->chroma_bleed     = (float)obs_data_get_double(settings, SETTING_CHROMA_BLEED);
}

static obs_properties_t *distort_filter_properties(void *data)
{
	obs_properties_t *props = obs_properties_create();

	obs_properties_add_float_slider(props, SETTING_PIXEL_SIZE, TEXT_PIXEL_SIZE, 1.0, 64.0, 1.0);
	obs_properties_add_float_slider(props, SETTING_RGB_SHIFT, TEXT_RGB_SHIFT, 0.0, 0.1, 0.001);
	obs_properties_add_float_slider(props, SETTING_CHROMA_BLEED, TEXT_CHROMA_BLEED, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_WAVE_AMOUNT, TEXT_WAVE_AMOUNT, 0.0, 0.05, 0.001);
	obs_properties_add_float_slider(props, SETTING_WAVE_SPEED, TEXT_WAVE_SPEED, 0.0, 20.0, 0.1);
	obs_properties_add_float_slider(props, SETTING_SCANLINE_INT, TEXT_SCANLINE_INT, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_INTERLACE, TEXT_INTERLACE, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_GLITCH_INTENSITY, TEXT_GLITCH_INTENSITY, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_PACKET_LOSS, TEXT_PACKET_LOSS, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_FREEZE_FRAMES, TEXT_FREEZE_FRAMES, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_FREEZE_DURATION, TEXT_FREEZE_DURATION, 0.0, 0.5, 0.01);
	obs_properties_add_float_slider(props, SETTING_RANDOMIZE_PIXEL, TEXT_RANDOMIZE_PIXEL, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_RANDOMIZE_WAVE, TEXT_RANDOMIZE_WAVE, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_RANDOMIZE_GLITCH, TEXT_RANDOMIZE_GLITCH, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_FLICKER, TEXT_FLICKER, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_GHOST, TEXT_GHOST, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_HORIZONTAL_HOLD, TEXT_HORIZONTAL_HOLD, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_POSTERIZE, TEXT_POSTERIZE, 0.0, 1.0, 0.01);
	obs_properties_add_float_slider(props, SETTING_ROLLING_BAR, TEXT_ROLLING_BAR, 0.0, 1.0, 0.01);

	UNUSED_PARAMETER(data);
	return props;
}

static void distort_filter_defaults(obs_data_t *settings)
{
	obs_data_set_default_double(settings, SETTING_PIXEL_SIZE, 8.0);
	obs_data_set_default_double(settings, SETTING_RGB_SHIFT, 0.02);
	obs_data_set_default_double(settings, SETTING_CHROMA_BLEED, 0.15);
	obs_data_set_default_double(settings, SETTING_WAVE_AMOUNT, 0.01);
	obs_data_set_default_double(settings, SETTING_WAVE_SPEED, 3.0);
	obs_data_set_default_double(settings, SETTING_SCANLINE_INT, 0.3);
	obs_data_set_default_double(settings, SETTING_INTERLACE, 0.2);
	obs_data_set_default_double(settings, SETTING_GLITCH_INTENSITY, 0.3);
	obs_data_set_default_double(settings, SETTING_PACKET_LOSS, 0.2);
	obs_data_set_default_double(settings, SETTING_FREEZE_FRAMES, 0.15);
	obs_data_set_default_double(settings, SETTING_FREEZE_DURATION, 0.08);
	obs_data_set_default_double(settings, SETTING_RANDOMIZE_PIXEL, 0.3);
	obs_data_set_default_double(settings, SETTING_RANDOMIZE_WAVE, 0.4);
	obs_data_set_default_double(settings, SETTING_RANDOMIZE_GLITCH, 0.5);
	obs_data_set_default_double(settings, SETTING_FLICKER, 0.15);
	obs_data_set_default_double(settings, SETTING_GHOST, 0.1);
	obs_data_set_default_double(settings, SETTING_HORIZONTAL_HOLD, 0.1);
	obs_data_set_default_double(settings, SETTING_POSTERIZE, 0.0);
	obs_data_set_default_double(settings, SETTING_ROLLING_BAR, 0.1);
}

static void distort_filter_tick(void *data, float seconds)
{
	struct distort_filter_data *filter = data;

	filter->time += seconds;
	filter->rand_seed = (float)os_gettime_ns() / 1000000000.0f;

	/* Freeze frame logic */
	if (filter->freeze_frames > 0.0f) {
		if (filter->freeze_active) {
			filter->freeze_timer += seconds;
			if (filter->freeze_timer >= filter->freeze_duration) {
				filter->freeze_active = false;
				filter->next_freeze = (1.0f - filter->freeze_frames) * 4.0f + 0.2f +
						     random_val(filter, 0.0f) * 2.0f;
			}
		} else {
			filter->next_freeze -= seconds;
			if (filter->next_freeze <= 0.0f) {
				filter->freeze_active = true;
				filter->freeze_timer = 0.0f;
				filter->freeze_time_value = filter->time;
				filter->freeze_seed_value = filter->rand_seed;
			}
		}
	} else {
		filter->freeze_active = false;
	}
}

static void distort_filter_render(void *data, gs_effect_t *effect)
{
	UNUSED_PARAMETER(effect);

	struct distort_filter_data *filter = data;
	obs_source_t *target = obs_filter_get_target(filter->context);

	if (!target) {
		obs_source_skip_video_filter(filter->context);
		return;
	}

	uint32_t width  = obs_source_get_base_width(target);
	uint32_t height = obs_source_get_base_height(target);

	if (!width || !height) {
		obs_source_skip_video_filter(filter->context);
		return;
	}

	if (obs_source_process_filter_begin(filter->context, GS_RGBA, OBS_ALLOW_DIRECT_RENDERING)) {
		struct vec2 uv_size;
		vec2_set(&uv_size, (float)width, (float)height);

		/* Effective time: frozen or advancing */
		float eff_time = filter->freeze_active ? filter->freeze_time_value : filter->time;
		float eff_seed = filter->freeze_active ? filter->freeze_seed_value : filter->rand_seed;

		/* Randomize parameters */
		float r_pixel = filter->pixel_size;
		if (filter->randomize_pixel > 0.0f) {
			float jitter = (random_val(filter, 1.0f) - 0.5f) * 2.0f *
				       filter->randomize_pixel * filter->pixel_size;
			r_pixel = fmaxf(1.0f, filter->pixel_size + jitter);
		}

		float r_wave_amt = filter->wave_amount;
		float r_wave_spd = filter->wave_speed;
		if (filter->randomize_wave > 0.0f) {
			float jitter = (random_val(filter, 3.0f) - 0.5f) * 2.0f *
				       filter->randomize_wave * filter->wave_amount;
			r_wave_amt = fmaxf(0.0f, filter->wave_amount + jitter);
			jitter = (random_val(filter, 4.0f) - 0.5f) * 2.0f *
				 filter->randomize_wave * filter->wave_speed;
			r_wave_spd = fmaxf(0.0f, filter->wave_speed + jitter);
		}

		float r_glitch = filter->glitch_int;
		if (filter->randomize_glitch > 0.0f) {
			float jitter = (random_val(filter, 5.0f) - 0.5f) * 2.0f *
				       filter->randomize_glitch;
			r_glitch = fmaxf(0.0f, fminf(1.0f, filter->glitch_int + jitter));
		}

		gs_effect_set_float(filter->param_pixel_size, r_pixel);
		gs_effect_set_float(filter->param_rgb_shift, filter->rgb_shift);
		gs_effect_set_float(filter->param_wave_amount, r_wave_amt);
		gs_effect_set_float(filter->param_wave_speed, r_wave_spd);
		gs_effect_set_float(filter->param_scanline_int, filter->scanline_int);
		gs_effect_set_float(filter->param_glitch_int, r_glitch);
		gs_effect_set_float(filter->param_time, eff_time);
		gs_effect_set_float(filter->param_rand_seed, eff_seed);
		gs_effect_set_vec2(filter->param_uv_size, &uv_size);
		gs_effect_set_float(filter->param_packet_loss, filter->packet_loss);
		gs_effect_set_float(filter->param_freeze_time, filter->freeze_active ? 1.0f : 0.0f);
		gs_effect_set_float(filter->param_interlace, filter->interlace);
		gs_effect_set_float(filter->param_flicker, filter->flicker);
		gs_effect_set_float(filter->param_ghost, filter->ghost);
		gs_effect_set_float(filter->param_horizontal_hold, filter->horizontal_hold);
		gs_effect_set_float(filter->param_posterize, filter->posterize);
		gs_effect_set_float(filter->param_rolling_bar, filter->rolling_bar);
		gs_effect_set_float(filter->param_chroma_bleed, filter->chroma_bleed);

		gs_blend_state_push();
		gs_blend_function(GS_BLEND_ONE, GS_BLEND_INVSRCALPHA);

		obs_source_process_filter_end(filter->context, filter->effect, width, height);

		gs_blend_state_pop();
	}
}

struct obs_source_info distort_filter = {
	.id = "distort_filter",
	.type = OBS_SOURCE_TYPE_FILTER,
	.output_flags = OBS_SOURCE_VIDEO,
	.get_name = distort_filter_get_name,
	.create = distort_filter_create,
	.destroy = distort_filter_destroy,
	.update = distort_filter_update,
	.get_properties = distort_filter_properties,
	.get_defaults = distort_filter_defaults,
	.video_tick = distort_filter_tick,
	.video_render = distort_filter_render,
};

OBS_DECLARE_MODULE()

OBS_MODULE_USE_DEFAULT_LOCALE("obs-distort-filter", "en-US")

const char *obs_module_description(void)
{
	return "Glitch / Distortion video filter for OBS Studio";
}

bool obs_module_load(void)
{
	/* Use obs_register_source_s with size 408 for backward compatibility
	 * with OBS versions that don't have filter_add/get_dark_icon/get_light_icon */
	obs_register_source_s(&distort_filter, 408);
	return true;
}
