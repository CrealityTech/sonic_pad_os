$(call inherit-product-if-exists, target/allwinner/v831-common/v831-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := v831_qaqc
PRODUCT_DEVICE := v831-qaqc
PRODUCT_MODEL := Allwinner v831 qaqc board
