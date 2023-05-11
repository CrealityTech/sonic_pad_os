$(call inherit-product-if-exists, target/allwinner/c600-common/c600-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := c600_cuckoo
PRODUCT_DEVICE := c600-cuckoo
PRODUCT_MODEL := SITAR F20A EVB
