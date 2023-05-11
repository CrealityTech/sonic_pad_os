$(call inherit-product-if-exists, target/allwinner/v533-common/v533-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := v533_cdr
PRODUCT_DEVICE := v533-cdr
PRODUCT_MODEL := Allwinner v533 sport dv board
