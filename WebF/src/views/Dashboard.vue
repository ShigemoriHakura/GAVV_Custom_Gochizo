<script setup>
import { ref, watch } from 'vue'
import ArgonButton from "@/components/ArgonButton.vue";
import GochizoTable from "./components/GochizoTable.vue";


import gochizo_default from "@/assets/img/gochizo/gochizo_default.png";

/*
//                            5.6k 6.8k 10k  12k  16k  20k  24k  30k  39k  47k  62k
const int RrX_final_array[] = {242, 239, 230, 225, 215, 204, 195, 179, 156, 135, 97};
*/
const gochizoArchive = {
  A017: {
    name: "多汁软糖",
    RrA: "97",
    RrB: "97",
    image: gochizo_default,
  },
  A018: {
    name: "拳击软糖",
    RrA: "97",
    RrB: "135",
    image: gochizo_default,
  },
  A099: {
    name: "假面骑士W",
    RrA: "225",
    RrB: "195",
    image: gochizo_default,
  },
};

const SERVICE = "3426d525-b6e9-4489-80f4-632e2251a2a6"
const STATUS = "3426d545-b6e9-4489-80f4-632e2251a2a6"


const isConnected = ref(false);
const serverD = ref();
const stCharacteristic = ref();

const RrA = ref(0);
const RrB = ref(0);
const Status = ref(0);
const zeroPad = (num, places) => String(num).padStart(places, '0')

watch(RrA, (newValue, oldValue) => {
  console.log('RrA Changed', newValue, oldValue)
  if(newValue < 0 || newValue > 255){
    console.log('RrA rollback')
    RrA.value = oldValue
  }
}, { immediate: true, deep: true })

watch(RrB, (newValue, oldValue) => {
  console.log('RrB Changed', newValue, oldValue)
  if(newValue < 0 || newValue > 255){
    console.log('RrB rollback')
    RrB.value = oldValue
  }
}, { immediate: true, deep: true })

function buf2hex(buffer) {
  return [...new Uint8Array(buffer)]
    .map(x => x.toString(16).padStart(2, '0'))
    .join('');
}

const decodeHex = (string) => {
  const uint8array = new Uint8Array(Math.ceil(string.length / 2));
  for (let i = 0; i < string.length;)
    uint8array[i / 2] = Number.parseInt(string.slice(i, i += 2), 16);
  return uint8array;
}

async function onScanDevice() {
  console.log('loading')
  navigator.bluetooth.requestDevice({
    filters: [{ services: [SERVICE] }],
  })
    .then(async device => {
      console.log('Name: ' + device.name)
      console.log(device)
      device.addEventListener('gattserverdisconnected', onDisconnected);
      await device.gatt.connect()
      console.log(device.gatt)
      return device.gatt.connect()
    })
    .then(server => {
      console.log("server")
      console.log(server)
      serverD.value = server
      return server.getPrimaryService(SERVICE)
    })
    .then(async service => {
      console.log("service")
      console.log(service)
      return await service.getCharacteristic(STATUS)
    })
    .then(async characteristics => {
      stCharacteristic.value = characteristics
      characteristics.startNotifications()
      characteristics.addEventListener(
        "characteristicvaluechanged",
        async (e) => {
          parseCharacteristic(e.target.value.buffer)
        }
      )
      return characteristics.readValue()
    })
    .then(characteristicValue => {
      console.log(characteristicValue)
      parseCharacteristic(characteristicValue.buffer)
    })
    //.then(characteristic => characteristic.addEventListener('characteristicvaluechanged', this.handleCharacteristicValueChanged.bind(this)))
    .catch(error => console.log(error));
}

function parseCharacteristic(buffer) {
  let hexString = buf2hex(buffer)
  console.log("characteristic: " + hexString)
  isConnected.value = true
  Status.value = parseInt(hexString.substring(0, 2))
  RrA.value = parseInt(hexString.substring(2, 4), 16)
  RrB.value = parseInt(hexString.substring(4, 6), 16)
}

function onDisconnected() {
  console.log('Device is disconnected.')
  isConnected.value = false
}

function calcSPIToRes(value) {
  return parseInt((255 - value) / 255 * 10000) / 100
}

function setDeviceStatus(status) {
  if (!isConnected.value) {
    return
  }
  var hexStr = "" + zeroPad(status, 2) + RrA.value.toString(16) + RrB.value.toString(16) + "00"
  console.log(hexStr)
  stCharacteristic.value.writeValueWithoutResponse(decodeHex(hexStr))
  if (status == 4) {
    isConnected.value = false
  }
}

function changeGozhicoValue(rA, rB) {
  if (!isConnected.value) {
    return
  }
  console.log("Set to:" + rA + "|" + rB)
  RrA.value = rA
  RrB.value = rB
  setDeviceStatus(1)
}

</script>

<template>
  <div class="py-4 container-fluid">
    <div class="row">

      <div class="col-lg-12">
        <GochizoTable></GochizoTable>
      </div>

      <div class="col-lg-12">
        <div class="row mt-4">
          <div class="col-lg-5">
            <div class="card">
              <div class="p-3 pb-0 card-header">
                <h6 class="mb-0">链接饱藏</h6>
              </div>
              <div class="p-3 card-body">


                <div class="row px-sm-3">
                  <argon-button @click="onScanDevice" size="md" variant="contained">搜索设备</argon-button>
                </div>

                <div v-if="isConnected">
                  <hr>
                  <h6 class="mb-0">设备信息</h6>
                  <h5 class="mb-0">RrA: {{ calcSPIToRes(RrA) }}k </h5>
                  <input class="form-control" v-model="RrA" placeholder="225">

                  <h5 class="mb-0">RrB: {{ calcSPIToRes(RrB) }}k </h5>
                  <input class="form-control" v-model="RrB" placeholder="195">
                  <br>
                  <argon-button @click="setDeviceStatus(1)" full-width color="warning"
                    variant="contained">设置饱藏</argon-button>

                  <hr>
                  <div class="row sm-4">
                    <div class="col-sm">
                      <argon-button full-width color="danger" variant="contained">断开链接</argon-button>
                      <br><br>
                    </div>
                    <div class="col-sm">
                      <argon-button @click="setDeviceStatus(2)" full-width color="primary"
                        variant="contained">插入饱藏</argon-button>
                      <br><br>
                    </div>
                    <div class="col-sm">
                      <argon-button @click="setDeviceStatus(3)" full-width color="primary"
                        variant="contained">拔出饱藏</argon-button>
                      <br><br>
                    </div>
                    <div class="col-sm">
                      <argon-button @click="setDeviceStatus(4)" full-width color="primary"
                        variant="contained">关闭饱藏</argon-button>
                      <br><br>
                    </div>
                  </div>
                </div>

              </div>
            </div>
          </div>
          <div class="col-lg-7 mb-lg-0 mb-4">
            <div class="card">
              <div class="p-3 pb-0 card-header">
                <div class="d-flex justify-content-between">
                  <h6 class="mb-2">已知饱藏列表</h6>
                </div>
              </div>
              <div class="table-responsive">
                <table class="table align-items-center">
                  <tbody>
                    <tr v-for="(gochizo, index) in gochizoArchive" :key="index">
                      <td class="w-30">
                        <div class="px-2 py-1 d-flex align-items-center">
                          <div>
                            <img :src="gochizo.image" alt="Gochizo Image" style="height: 50px;" />
                          </div>
                          <div class="ms-4">
                            <p class="mb-0 text-xs font-weight-bold">
                              名称:
                            </p>
                            <h6 class="mb-0 text-sm">{{ gochizo.name }}</h6>
                          </div>
                        </div>
                      </td>
                      <td>
                        <div class="text-center">
                          <p class="mb-0 text-xs font-weight-bold">RrA:(A电阻)</p>
                          <h6 class="mb-0 text-sm">{{ gochizo.RrA }}({{ calcSPIToRes(gochizo.RrA) }})</h6>
                        </div>
                      </td>
                      <td>
                        <div class="text-center">
                          <p class="mb-0 text-xs font-weight-bold">RrB:(A电阻)</p>
                          <h6 class="mb-0 text-sm">{{ gochizo.RrB }}({{ calcSPIToRes(gochizo.RrB) }})</h6>
                        </div>
                      </td>
                      <td>
                        <div class="text-center">
                          <argon-button color="primary" variant="contained"
                            @click="changeGozhicoValue(parseInt(gochizo.RrA), parseInt(gochizo.RrB))">点我设置</argon-button>
                        </div>
                      </td>
                    </tr>
                  </tbody>
                </table>
              </div>
            </div>
          </div>
        </div>
      </div>





    </div>
  </div>
</template>
