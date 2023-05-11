$(call inherit-product-if-exists, target/allwinner/h6-common/h6-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := h6_p1_axp806
PRODUCT_DEVICE := h6-p1_axp806
PRODUCT_MODEL := H6
