$(call inherit-product-if-exists, target/allwinner/v316-common/v316-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := v316_perfnor
PRODUCT_DEVICE := v316-perfnor
PRODUCT_MODEL := Allwinner v316 sport dv board
