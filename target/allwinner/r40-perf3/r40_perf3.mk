$(call inherit-product-if-exists, target/allwinner/r40-common/r40-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r40_perf3
PRODUCT_DEVICE := r40-perf3
PRODUCT_MODEL := Azalea R40 Perf3
