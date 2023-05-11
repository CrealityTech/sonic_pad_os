$(call inherit-product-if-exists, target/allwinner/r7-common/r7-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := camerabox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r7_R7_pref1
PRODUCT_DEVICE := r7-R7_pref1
PRODUCT_MODEL := BANJO R7_pref1
