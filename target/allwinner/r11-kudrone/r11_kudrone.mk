$(call inherit-product-if-exists, target/allwinner/psw-common/psw-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r11_kudrone
PRODUCT_DEVICE := r11-kudrone
PRODUCT_MODEL := BANJO Kudrone
