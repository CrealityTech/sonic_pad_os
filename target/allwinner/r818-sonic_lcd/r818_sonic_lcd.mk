$(call inherit-product-if-exists, target/allwinner/r818-common/r818-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r818_sonic_lcd
PRODUCT_DEVICE := r818-sonic_lcd
PRODUCT_MODEL := R818 SONIC HDMI
