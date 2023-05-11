$(call inherit-product-if-exists, target/allwinner/h6-common/h616-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := h616_vs
PRODUCT_DEVICE := h616-vs
PRODUCT_MODEL := H616
