ESP_SR_PATH=~/ghq/github.com/espressif/esp-sr
SDKCONFIG_PATH=~/tmp/sr_model_proj/sr_model_proj/sdkconfig
python ${ESP_SR_PATH}/model/movemodel.py -d1 ${SDKCONFIG_PATH} -d2 ${ESP_SR_PATH} -d3 ./srmodels_build/
