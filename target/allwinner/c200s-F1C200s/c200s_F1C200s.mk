$(call inherit-product-if-exists, target/allwinner/c200s-common/c200s-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := c200s_F1C200s
PRODUCT_DEVICE := c200s-F1C200s
PRODUCT_MODEL := SITAR F20A EVB
