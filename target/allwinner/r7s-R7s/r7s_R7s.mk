$(call inherit-product-if-exists, target/allwinner/r7s-common/r7s-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := camerabox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r7s_R7s
PRODUCT_DEVICE := r7s-R7s
PRODUCT_MODEL := LUTE R7s
