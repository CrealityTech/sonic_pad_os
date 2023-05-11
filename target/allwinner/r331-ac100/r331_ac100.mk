$(call inherit-product-if-exists, target/allwinner/r331-common/r331-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r331_ac100
PRODUCT_DEVICE := r331-ac100
PRODUCT_MODEL := Allwinner r331 ac100 board
