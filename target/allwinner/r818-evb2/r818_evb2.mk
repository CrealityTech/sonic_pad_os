$(call inherit-product-if-exists, target/allwinner/r818-common/r818-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r818_evb2
PRODUCT_DEVICE := r818-evb2
PRODUCT_MODEL := R818 EVB2
