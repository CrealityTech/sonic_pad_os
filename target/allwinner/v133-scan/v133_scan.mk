$(call inherit-product-if-exists, target/allwinner/v133-common/v133-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := v133_scan
PRODUCT_DEVICE := v133-scan
PRODUCT_MODEL := SITAR F20A EVB
