$(call inherit-product-if-exists, target/allwinner/r311-common/r311-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r311_perf1
PRODUCT_DEVICE := r311-perf1
PRODUCT_MODEL := r311 R311 perf1 board
