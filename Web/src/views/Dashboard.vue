<script setup>
import { ref, watch } from 'vue'
import ArgonButton from "@/components/ArgonButton.vue";
import GochizoTable from "./components/GochizoTable.vue";

import gochizoArchive from "../gochizoList.js"

/*
//                            5.6k  6.8k 8.2k 10k  12k  16k  20k  24k  30k  39k  47k  62k
const int RrX_final_array[] = {242, 239, 236, 231, 226, 215, 206, 196, 181, 156, 135, 97};
*/


const SERVICE = "3426d525-b6e9-4489-80f4-632e2251a2a6"
const STATUS = "3426d545-b6e9-4489-80f4-632e2251a2a6"
const SERIAL = "3426d546-b6e9-4489-80f4-632e2251a2a6"

const isConnected = ref(false);
const serverD = ref();
const statusCharacteristic = ref();
const serialCharacteristic = ref();

const RrA = ref(0);
const RrB = ref(0);
const Status = ref(0);
const Serial = ref(0);
const Sleep = ref(0);
const Random = ref(0);
const zeroPad = (num, places) => String(num).padStart(places, '0')

watch(RrA, (newValue, oldValue) => {
  console.log('RrA Changed', newValue, oldValue)
  if(newValue < 0 || newValue > 256){
    console.log('RrA rollback')
    RrA.value = oldValue
  }
}, { immediate: true, deep: true })

watch(RrB, (newValue, oldValue) => {
  console.log('RrB Changed', newValue, oldValue)
  if(newValue < 0 || newValue > 256){
    console.log('RrB rollback')
    RrB.value = oldValue
  }
}, { immediate: true, deep: true })

watch(Sleep, (newValue, oldValue) => {
  console.log('Sleep Changed', newValue, oldValue)
  if(newValue < 0 || newValue > 256){
    console.log('Sleep rollback')
    Sleep.value = oldValue
  }
}, { immediate: true, deep: true })

watch(Random, (newValue, oldValue) => {
  console.log('Random Changed', newValue, oldValue)
  if(newValue < 0 || newValue > 256){
    console.log('Random rollback')
    Random.value = oldValue
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
      return Promise.all([
        service.getCharacteristic(STATUS),
        service.getCharacteristic(SERIAL)
      ])
    })
    .then(async characteristics => {
      console.log(characteristics)
      statusCharacteristic.value = characteristics[0]
      serialCharacteristic.value = characteristics[1]
      characteristics[0].startNotifications()
      characteristics[0].addEventListener(
        "characteristicvaluechanged",
        async (e) => {
          parseCharacteristic(e.target.value.buffer)
        }
      )
      return Promise.all([
        characteristics[0].readValue(),
        characteristics[1].readValue(),
      ])
    })
    .then(characteristicValue => {
      console.log(characteristicValue[0].buffer)
      parseCharacteristic(characteristicValue[0].buffer)
      Serial.value = new TextDecoder().decode(characteristicValue[1].buffer).toUpperCase()
    })
    //.then(characteristic => characteristic.addEventListener('characteristicvaluechanged', this.handleCharacteristicValueChanged.bind(this)))
    .catch(error => console.log(error));
}

function parseCharacteristic(buffer) {
  let hexString1 = buf2hex(buffer)
  console.log("buf2hex: " + hexString1)
  let hexString = new TextDecoder().decode(buffer).toUpperCase()
  console.log("characteristic: " + hexString)
  isConnected.value = true
  Status.value = parseInt(hexString.substring(0, 2))
  RrA.value = parseInt(hexString.substring(2, 4), 16)
  RrB.value = parseInt(hexString.substring(4, 6), 16)
  Sleep.value = parseInt(hexString.substring(6, 8), 16)
  console.log(hexString.substring(8, 10))
  Random.value = parseInt(hexString.substring(8, 10))
}

function onDisconnected() {
  console.log('Device is disconnected.')
  isConnected.value = false
}

function disconnectGochizo(){
  if (!isConnected.value) {
    return
  }
  serverD.value.disconnect()
  isConnected.value = false
}

function calcSPIToRes(value) {
  return parseInt((256 - value) / 256 * 10000) / 100
}

function setDeviceStatus(status) {
  if (!isConnected.value) {
    return
  }
  var hexStr = "" + 
  zeroPad(status, 2) + 
  parseInt(RrA.value, 10).toString(16) + 
  parseInt(RrB.value, 10).toString(16) + 
  parseInt(Sleep.value, 10).toString(16) + 
  zeroPad(Random.value, 2)
  console.log(hexStr)
  statusCharacteristic.value.writeValueWithoutResponse(decodeHex(hexStr))
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

function printAvailableGochizo(){
  //console.log(gochizoArchive.gochizoArchive)
  let arrayA = gochizoArchive.gochizoArchive
  var str = ""
  for (var element in arrayA){
    str += `{` + arrayA[element].RrA + `, ` + arrayA[element].RrB + `}, \r\n`
  }
  console.log(str)
  //gochizoArchive.gochizoArchive
  return str
}

printAvailableGochizo()

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
                  <h5 class="mb-0">序列号: {{ Serial }} </h5>
                  <h5 class="mb-0">RrA: {{ calcSPIToRes(RrA) }}k </h5>
                  <input class="form-control" v-model="RrA" placeholder="226">
                  <h5 class="mb-0">RrB: {{ calcSPIToRes(RrB) }}k </h5>
                  <input class="form-control" v-model="RrB" placeholder="196">
                  <h5 class="mb-0">睡眠延时: {{ Sleep }}秒(0-255) </h5>
                  <input class="form-control" v-model="Sleep" placeholder="60">
                  <h5 class="mb-0">开启随机: {{ Random }}(0关闭，1开启) </h5>
                  <input class="form-control" v-model="Random" placeholder="0">
                  <br>
                  <argon-button @click="setDeviceStatus(1)" full-width color="warning"
                    variant="contained">设置饱藏</argon-button>

                  <hr>
                  <div class="row sm-4">
                    <div class="col-sm">
                      <argon-button @click="disconnectGochizo()" full-width color="danger" variant="contained">断开链接</argon-button>
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
                    <tr v-for="(gochizo, index) in gochizoArchive.gochizoArchive" :key="index">
                      <td class="w-30">
                        <div class="px-2 py-1 d-flex align-items-center">
                          <div>
                            <img :src="gochizo.image" alt="Gochizo Image" style="height: 50px;" />
                          </div>
                          <div class="ms-4">
                            <p class="mb-0 text-xs font-weight-bold">
                              名称:
                            </p>
                            <h6 class="mb-0 text-sm">({{ index }}){{ gochizo.name }}</h6>
                          </div>
                        </div>
                      </td>
                      <td>
                        <div class="text-center">
                          <p class="mb-0 text-xs font-weight-bold">RrA:(A电阻)</p>
                          <h6 class="mb-0 text-sm">{{ gochizo.RrA }}({{ calcSPIToRes(gochizo.RrA) }}k)</h6>
                        </div>
                      </td>
                      <td>
                        <div class="text-center">
                          <p class="mb-0 text-xs font-weight-bold">RrB:(B电阻)</p>
                          <h6 class="mb-0 text-sm">{{ gochizo.RrB }}({{ calcSPIToRes(gochizo.RrB) }}k)</h6>
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
