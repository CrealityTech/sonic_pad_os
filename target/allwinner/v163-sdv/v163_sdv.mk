$(call inherit-product-if-exists, target/allwinner/v163-common/v163-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := v163_sdv
PRODUCT_DEVICE := v163-sdv
PRODUCT_MODEL := Allwinner v163 sport dv board
