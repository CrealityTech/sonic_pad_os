$(call inherit-product-if-exists, target/allwinner/r6-common/r6-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r6_perf1
PRODUCT_DEVICE := r6-perf1
PRODUCT_MODEL := SITAR F20A PERF1
