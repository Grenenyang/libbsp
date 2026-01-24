deps_config := \
	/data/hengyang/work/wt_eth_tools/wt_eth_tools/ksource/Kconfig \
	/data/hengyang/work/wt_eth_tools/wt_eth_tools/usource/Kconfig \
	/data/hengyang/work/wt_eth_tools/wt_eth_tools/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(PRJ_ROOT)" "/data/hengyang/work/wt_eth_tools/wt_eth_tools"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
