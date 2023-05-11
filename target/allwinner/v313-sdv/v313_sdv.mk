$(call inherit-product-if-exists, target/allwinner/v313-common/v313-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := v313_sdv
PRODUCT_DEVICE := v313-sdv
PRODUCT_MODEL := Allwinner v313 sport dv board
