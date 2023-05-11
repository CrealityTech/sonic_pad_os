$(call inherit-product-if-exists, target/allwinner/h3-common/h3-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := h3_p1
PRODUCT_DEVICE := h3-p1
PRODUCT_MODEL := H3
