$(call inherit-product-if-exists, target/allwinner/r329-common/r329-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r329_evb1
PRODUCT_DEVICE := r329-evb1
PRODUCT_MODEL := R329 evb1
