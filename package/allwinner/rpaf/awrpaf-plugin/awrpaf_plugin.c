#include <alsa/asoundlib.h>
#include <alsa/pcm.h>
#include <alsa/conf.h>
#include <alsa/pcm_external.h>

#include <awrpaf_component.h>

typedef struct algo_desc {
	char algo_name[32];
	int algo_type;
	int algo_index;
} algo_desc_t;

struct awrpaf_info {
	snd_pcm_extplug_t ext;
	algo_desc_t *algos_desc;
	unsigned int algos_desc_num;
	unsigned int enable;
	int dspid;
	int stream;
	char card_name[32];
};

static void algos_control(struct awrpaf_info *info, unsigned int enable)
{
	int i, ret;
	algo_desc_t *algo = NULL;

	for (i = 0; i < info->algos_desc_num; i++) {
		algo = &info->algos_desc[i];
		/*printf("[%s] line:%d algo_name=%s, algo_type=%d, algo_index=%d\n", __func__, __LINE__,*/
			/*algo->algo_name, algo->algo_type, algo->algo_index);	*/
		ret = awrpaf_component_algo_config_set(info->dspid, info->card_name,
				algo->algo_index, algo->algo_type,
				info->stream, enable);
		if (ret != 0) {
			SNDERR("algo[name:%s, type:%d] %s failed, return %d",
				algo->algo_name, algo->algo_type,
				enable ? "enable" : "disable", ret);
		}
	}
}

static int awrpaf_init(snd_pcm_extplug_t *ext)
{
#if 0
	struct awrpaf_info *info = (struct awrpaf_info *)ext;

	algos_control(info, info->enable);
#endif
	return 0;
}

static snd_pcm_sframes_t awrpaf_transfer(snd_pcm_extplug_t *ext,
				const snd_pcm_channel_area_t *dst_areas,
				snd_pcm_uframes_t dst_offset,
				const snd_pcm_channel_area_t *src_areas,
				snd_pcm_uframes_t src_offset,
				snd_pcm_uframes_t size)
{
	struct awrpaf_info *info = (struct awrpaf_info *)ext;

	snd_pcm_areas_copy(dst_areas, dst_offset, src_areas, src_offset,
				   ext->slave_channels, size, ext->slave_format);
	return size;
}

static int awrpaf_close(snd_pcm_extplug_t *ext)
{
	struct awrpaf_info *info = (struct awrpaf_info *)ext;

	free(info->algos_desc);
	info->algos_desc = NULL;
	info->algos_desc_num = 0;
	return 0;
}

static void awrpaf_dump(snd_pcm_extplug_t *ext, snd_output_t *out)
{
	struct awrpaf_info *info = (struct awrpaf_info *)ext;
	int i;
	algo_desc_t *algo = NULL;

	snd_output_printf(out, "%s:\n", info->ext.name);
	for (i = 0; i < info->algos_desc_num; i++) {
		algo = &info->algos_desc[i];
		snd_output_printf(out, "algo_index(%d)--->algo_type(%d)[%s]\n",
			algo->algo_index, algo->algo_type, algo->algo_name);
	}
}

static struct snd_pcm_extplug_callback awrpaf_callback = {
	.transfer	= awrpaf_transfer,
	.close		= awrpaf_close,
	.hw_params	= NULL,
	.hw_free	= NULL,
	.dump		= awrpaf_dump,
	.init		= awrpaf_init,
	.query_chmaps	= NULL,
	.get_chmap	= NULL,
	.set_chmap	= NULL,
};

static void awrpaf_free(struct awrpaf_info *info)
{
	free(info);
}

/* define in libasound.so */
extern int safe_strtol(const char *str, long *val);
SND_PCM_PLUGIN_DEFINE_FUNC(awrpaf)
{
	int err;
	snd_config_iterator_t i, next;
	snd_config_t *sconf = NULL;
	struct awrpaf_info *info = NULL;
	snd_config_t * algos_desc_config = NULL;
	algo_desc_t *algos_desc = NULL;
	long enable = 0, dspid = 0;
	const char *card_name;
	unsigned int idx;

	snd_config_for_each(i, next, conf) {
		snd_config_t *n = snd_config_iterator_entry(i);
		const char *id;
		if (snd_config_get_id(n, &id) < 0)
			continue;
		if (strcmp(id, "comment") == 0 || strcmp(id, "type") == 0)
			continue;
		if (strcmp(id, "slave") == 0) {
			sconf = n;
			continue;
		}
		if (strcmp(id, "algos_desc") == 0) {
			if (snd_config_get_type(n) != SND_CONFIG_TYPE_COMPOUND) {
				SNDERR("Invalid type for %s", id);
				return -EINVAL;
			}
			algos_desc_config = n;
			continue;
		}
		if (strcmp(id, "enable") == 0) {
			err = snd_config_get_integer(n, &enable);
			if (err < 0) {
				SNDERR("Invalid enable value %s", id);
				return -EINVAL;
			}
			continue;
		}
		if (strcmp(id, "dspid") == 0) {
			err = snd_config_get_integer(n, &dspid);
			if (err < 0) {
				SNDERR("Invalid enable value %s", id);
				return -EINVAL;
			}
			continue;
		}
		if (strcmp(id, "card_name") == 0) {
			err = snd_config_get_string(n, &card_name);
			if (err < 0) {
				SNDERR("Invalid enable value %s", id);
				return -EINVAL;
			}
			continue;
		}
		SNDERR("Unknown field %s", id);
		return -EINVAL;
	}

	if (!sconf) {
		SNDERR("No slave defined for awrpaf");
		return -EINVAL;
	}

	if (!algos_desc_config) {
		SNDERR("algos_desc is not defined");
		return -EINVAL;
	}

	unsigned int algo_count = 0;
	snd_config_for_each(i, next, algos_desc_config) {
		long index;
		snd_config_t *m = snd_config_iterator_entry(i);
		const char *id;
		if (snd_config_get_id(m, &id) < 0)
			continue;
		err = safe_strtol(id, &index);
		if (err < 0 || index < 0) {
			SNDERR("Invalid algo index: %s", id);
			return -EINVAL;
		}
		algo_count++;
	}

	algos_desc = calloc(algo_count, sizeof(algo_desc_t));
	if (!algos_desc) {
		err = -ENOMEM;
		goto _free;
	}

	idx = 0;
	for (idx = 0; idx < algo_count; ++idx) {
		algos_desc[idx].algo_type = -1;
		algos_desc[idx].algo_index = -1;
	}

	idx = 0;
	snd_config_for_each(i, next, algos_desc_config) {
		long algo_index, algo_type;
		snd_config_iterator_t j, jnext;
		snd_config_t *m = snd_config_iterator_entry(i);
		const char *id;
		const char *name;
		if (snd_config_get_id(m, &id) < 0)
			continue;
		err = safe_strtol(id, &algo_index);
		if (err < 0 || algo_index < 0) {
			SNDERR("Invalid algo index: %s", id);
			err = -EINVAL;
			goto _free;
		}
		algos_desc[idx].algo_index = algo_index;
		snd_config_for_each(j, jnext, m) {
			snd_config_t *n = snd_config_iterator_entry(j);
			const char *id;
			if (snd_config_get_id(n, &id) < 0)
				continue;
			if (strcmp(id, "comment") == 0)
				continue;
			if (strcmp(id, "algo_name") == 0) {
				err = snd_config_get_string(n, &name);
				if (err < 0) {
					SNDERR("Invalid value for %s", id);
					goto _free;
				}
				snprintf(algos_desc[idx].algo_name,
					sizeof(algos_desc[idx].algo_name),
					"%s", name);
				continue;
			}
			if (strcmp(id, "algo_type") == 0) {
				err = snd_config_get_integer(n, &algo_type);
				if (err < 0) {
					SNDERR("Invalid type for %s", id);
					goto _free;
				}
				algos_desc[idx].algo_type = algo_type;
				continue;
			}

			SNDERR("Unknown field %s", id);
			err = -EINVAL;
			goto _free;
		}
		idx++;
	}

	info = calloc(1, sizeof(*info));
	if (!info)
		return -ENOMEM;
	info->ext.version = SND_PCM_EXTPLUG_VERSION;
	info->ext.name = "AW RPAF Plugin "AWRPAF_VERSION;
	info->ext.callback = &awrpaf_callback;
	info->ext.private_data = info;

	info->algos_desc = algos_desc;
	info->algos_desc_num = algo_count;
	info->enable = (unsigned int)enable;
	info->dspid = (unsigned int)dspid;
	info->stream = stream;
	strncpy(info->card_name, card_name, sizeof(info->card_name));

	/* enable algo before open slave pcm */
	algos_control(info, info->enable);

	err = snd_pcm_extplug_create(&info->ext, name, root, sconf, stream, mode);
	if (err < 0) {
		SNDERR("create extplug failed");
		goto _free;
	}

	/*snd_pcm_extplug_set_param_minmax();*/

	*pcmp = info->ext.pcm;
	return 0;
_free:
	if (info != NULL)
		free(info);
	if (algos_desc != NULL)
		free(algos_desc);
	return err;
}
SND_PCM_PLUGIN_SYMBOL(awrpaf);
