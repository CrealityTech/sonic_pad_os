$(call inherit-product-if-exists, target/allwinner/v306-common/v306-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := v306_h300
PRODUCT_DEVICE := v306-h300
PRODUCT_MODEL := Allwinner v306 picture book robot board
