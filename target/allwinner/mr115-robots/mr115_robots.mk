$(call inherit-product-if-exists, target/allwinner/mr115-common/mr115-common.mk)

PRODUCT_PACKAGES +=

PRODUCT_COPY_FILES +=

PRODUCT_AAPT_CONFIG := large xlarge hdpi xhdpi
PRODUCT_AAPT_PERF_CONFIG := xhdpi
PRODUCT_CHARACTERISTICS := musicbox

PRODUCT_BRAND := allwinner
PRODUCT_NAME := mr115_robots
PRODUCT_DEVICE := mr115-robots
PRODUCT_MODEL := mr115 MR115 robots board
