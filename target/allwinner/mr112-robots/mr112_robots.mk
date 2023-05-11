$(call inherit-product-if-exists, target/allwinner/mr112-common/mr112-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := mr112_robots
PRODUCT_DEVICE := mr112-robots
PRODUCT_MODEL := Allwinner mr112 robots board
