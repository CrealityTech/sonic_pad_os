$(call inherit-product-if-exists, target/allwinner/perf1-common/perf1-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := g102_perf1
PRODUCT_DEVICE := g102-perf1
PRODUCT_MODEL := CELLO PERF1
