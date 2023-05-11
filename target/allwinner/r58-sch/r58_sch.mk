$(call inherit-product-if-exists, target/allwinner/r58-common/r58-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r58_sch
PRODUCT_DEVICE := r58-sch
PRODUCT_MODEL := Octopus R58 Dev
