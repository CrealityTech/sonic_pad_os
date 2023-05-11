$(call inherit-product-if-exists, target/allwinner/r328s2-common/r328s2-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r328s2_ubidemo
PRODUCT_DEVICE := r328s2-ubidemo
PRODUCT_MODEL := Allwinner r328s2 ubidemo board
