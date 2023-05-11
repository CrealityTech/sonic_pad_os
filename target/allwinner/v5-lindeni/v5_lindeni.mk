$(call inherit-product-if-exists, target/allwinner/v5-common/v5-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := v5_lindeni
PRODUCT_DEVICE := v5-lindeni
PRODUCT_MODEL := Allwinner v5 picture book robot board
