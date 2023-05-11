$(call inherit-product-if-exists, target/allwinner/r11-common/r11-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := camerabox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r11_mic
PRODUCT_DEVICE := r11-mic
PRODUCT_MODEL := BANJO MIC
