$(call inherit-product-if-exists, target/allwinner/r30-common/r30-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := Allwinner
PRODUCT_NAME := r30_std
PRODUCT_DEVICE := r30-std
PRODUCT_MODEL := r30 R30 standard board
