$(call inherit-product-if-exists, target/allwinner/r328s3-common/r328s3-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := r328s3_std_xr829
PRODUCT_DEVICE := r328s3-std_xr829
PRODUCT_MODEL := Allwinner r328s3 std xr829 board
