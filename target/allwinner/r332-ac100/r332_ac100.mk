$(call inherit-product-if-exists, target/allwinner/r332-common/r332-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r332_ac100
PRODUCT_DEVICE := r332-ac100
PRODUCT_MODEL := Allwinner r332 ac100 board
