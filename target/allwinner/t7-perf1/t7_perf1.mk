$(call inherit-product-if-exists, target/allwinner/t7-common/t7-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := t7_perf1
PRODUCT_DEVICE := t7-perf1
PRODUCT_MODEL := t7 T7 perf1 board
