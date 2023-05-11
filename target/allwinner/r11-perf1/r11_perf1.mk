$(call inherit-product-if-exists, target/allwinner/psw-common/psw-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r11_perf1
PRODUCT_DEVICE := r11-perf1
PRODUCT_MODEL := BANJO Perf1
