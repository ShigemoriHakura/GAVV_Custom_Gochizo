<script setup>
import { ref, watch } from 'vue'
import ArgonButton from "@/components/ArgonButton.vue";
import GochizoTable from "./components/GochizoTable.vue";


import gochizo_default from "@/assets/img/gochizo/gochizo_default.png";

/*
//                            5.6k  6.8k 8.2k 10k  12k  16k  20k  24k  30k  39k  47k  62k
const int RrX_final_array[] = {242, 239, 236, 231, 226, 215, 206, 196, 181, 156, 135, 97};
*/
const gochizoArchive = {
  A017: {
    name: "跳跃软糖（多汁）",
    RrA: "97",
    RrB: "97",
    image: gochizo_default,
  },
  A018: {
    name: "飞踢软糖",
    RrA: "97",
    RrB: "135",
    image: gochizo_default,
  },
  A019: {
    name: "拳击软糖",
    RrA: "97",
    RrB: "156",
    image: gochizo_default,
  },
  A020: {
    name: "钓鱼软糖",
    RrA: "97",
    RrB: "181",
    image: gochizo_default,
  },
  A021: {
    name: "惊异软糖",
    RrA: "97",
    RrB: "196",
    image: gochizo_default,
  },
  A022: {
    name: "火花软糖",
    RrA: "97",
    RrB: "206",
    image: gochizo_default,
  },
  A023: {
    name: "脆脆薯片（脆脆）",
    RrA: "97",
    RrB: "215",
    image: gochizo_default,
  },
  A026: {
    name: "火辣薯片",
    RrA: "97",
    RrB: "236",
    image: gochizo_default,
  },
  A027: {
    name: "巧克弹（啪唧啪唧）",
    RrA: "97",
    RrB: "239",
    image: gochizo_default,
  },
  A028: {
    name: "巧克咚",
    RrA: "97",
    RrB: "242",
    image: gochizo_default,
  },
  A030: {
    name: "巧克比",
    RrA: "135",
    RrB: "135",
    image: gochizo_default,
  },
  A031: {
    name: "挖宝巧克力（啪唧啪唧）",
    RrA: "135",
    RrB: "156",
    image: gochizo_default,
  },
  A032: {
    name: "巧库力（啪唧啪唧）",
    RrA: "135",
    RrB: "181",
    image: gochizo_default,
  },
  A033: {
    name: "巧库力",
    RrA: "135",
    RrB: "196",
    image: gochizo_default,
  },
  A034: {
    name: "软棉花糖（软绵绵）",
    RrA: "135",
    RrB: "206",
    image: gochizo_default,
  },
  A035: {
    name: "圆花糖",
    RrA: "135",
    RrB: "215",
    image: gochizo_default,
  },
  A036: {
    name: "涡轮糖果",
    RrA: "135",
    RrB: "226",
    image: gochizo_default,
  },
  A037: {
    name: "转圈糖果（舔一舔）",
    RrA: "135",
    RrB: "231",
    image: gochizo_default,
  },
  A038: {
    name: "爆糖果",
    RrA: "135",
    RrB: "236",
    image: gochizo_default,
  },
  A039: {
    name: "气泡汽水糖",
    RrA: "135",
    RrB: "239",
    image: gochizo_default,
  },
  A040: {
    name: "气泡汽水糖（咯嚓咯嚓）",
    RrA: "135",
    RrB: "242",
    image: gochizo_default,
  },
  A044: {
    name: "毛绒跳跳糖（毛茸茸）",
    RrA: "156",
    RrB: "181",
    image: gochizo_default,
  },
  A047: {
    name: "爆响米花（砰砰）",
    RrA: "156",
    RrB: "215",
    image: gochizo_default,
  },
  A049: {
    name: "圆饼（咯嚓咯嚓）",
    RrA: "156",
    RrB: "231",
    image: gochizo_default,
  },
  A050: {
    name: "摇晃果冻（摇摇晃晃）",
    RrA: "156",
    RrB: "236",
    image: gochizo_default,
  },
  A051: {
    name: "优雅马卡龙（高贵）",
    RrA: "156",
    RrB: "239",
    image: gochizo_default,
  },
  A052: {
    name: "甜甜圈（毛茸茸）",
    RrA: "156",
    RrB: "242",
    image: gochizo_default,
  },
  A053: {
    name: "焦焦奶糖（咀嚼咀嚼）",
    RrA: "181",
    RrB: "97",
    image: gochizo_default,
  },
  A054: {
    name: "曲奇奇（咯嚓咯嚓）",
    RrA: "181",
    RrB: "135",
    image: gochizo_default,
  },
  A056: {
    name: "大布丁（摇摇晃晃）",
    RrA: "181",
    RrB: "181",
    image: gochizo_default,
  },
  A059: {
    name: "毛绒跳跳糖",
    RrA: "181",
    RrB: "215",
    image: gochizo_default,
  },
  A062: {
    name: "爆响米花",
    RrA: "181",
    RrB: "236",
    image: gochizo_default,
  },
  A064: {
    name: "圆饼",
    RrA: "181",
    RrB: "242",
    image: gochizo_default,
  },
  A065: {
    name: "摇晃果冻",
    RrA: "196",
    RrB: "97",
    image: gochizo_default,
  },
  A066: {
    name: "优雅马卡龙",
    RrA: "196",
    RrB: "135",
    image: gochizo_default,
  },
  A067: {
    name: "甜甜圈",
    RrA: "196",
    RrB: "156",
    image: gochizo_default,
  },
  A068: {
    name: "焦焦奶糖",
    RrA: "196",
    RrB: "181",
    image: gochizo_default,
  },
  A069: {
    name: "曲奇奇",
    RrA: "196",
    RrB: "196",
    image: gochizo_default,
  },
  A071: {
    name: "大布丁",
    RrA: "196",
    RrB: "215",
    image: gochizo_default,
  },
  A073: {
    name: "射击零食（脆脆）",
    RrA: "196",
    RrB: "231",
    image: gochizo_default,
  },
  A074: {
    name: "尖刺零食（脆脆）",
    RrA: "196",
    RrB: "236",
    image: gochizo_default,
  },
  A075: {
    name: "下午茶点心（啪唧啪唧）",
    RrA: "196",
    RrB: "239",
    image: gochizo_default,
  },
  A076: {
    name: "点心（啪唧啪唧）",
    RrA: "196",
    RrB: "232",
    image: gochizo_default,
  },
  A077: {
    name: "巧克弹",
    RrA: "206",
    RrB: "97",
    image: gochizo_default,
  },
  A078: {
    name: "射击零食",
    RrA: "206",
    RrB: "135",
    image: gochizo_default,
  },
  A079: {
    name: "尖刺零食",
    RrA: "206",
    RrB: "156",
    image: gochizo_default,
  },
  A080: {
    name: "挖宝巧克力",
    RrA: "206",
    RrB: "181",
    image: gochizo_default,
  },
  A081: {
    name: "点心",
    RrA: "206",
    RrB: "196",
    image: gochizo_default,
  },
  A082: {
    name: "巧克弹",
    RrA: "206",
    RrB: "206",
    image: gochizo_default,
  },
  A083: {
    name: "土豆扫尾（脆脆）",
    RrA: "206",
    RrB: "215",
    image: gochizo_default,
  },
  A084: {
    name: "盾牌零食（脆脆）",
    RrA: "206",
    RrB: "226",
    image: gochizo_default,
  },
  A085: {
    name: "土豆扫尾",
    RrA: "206",
    RrB: "231",
    image: gochizo_default,
  },
  A086: {
    name: "盾牌扫尾",
    RrA: "206",
    RrB: "236",
    image: gochizo_default,
  },
  A089: {
    name: "特殊",
    RrA: "215",
    RrB: "97",
    image: gochizo_default,
  },
  A090: {
    name: "庆祝",
    RrA: "215",
    RrB: "135",
    image: gochizo_default,
  },
  A091: {
    name: "圣诞",
    RrA: "215",
    RrB: "156",
    image: gochizo_default,
  },
  A092: {
    name: "新年快乐",
    RrA: "215",
    RrB: "181",
    image: gochizo_default,
  },
  A093: {
    name: "音乐",
    RrA: "215",
    RrB: "196",
    image: gochizo_default,
  },
  A094: {
    name: "庆祝",
    RrA: "215",
    RrB: "206",
    image: gochizo_default,
  },
  A095: {
    name: "假面骑士空我",
    RrA: "215",
    RrB: "215",
    image: gochizo_default,
  },
  A096: {
    name: "假面骑士亚吉陀",
    RrA: "215",
    RrB: "226",
    image: gochizo_default,
  },
  A097: {
    name: "假面骑士龙骑",
    RrA: "215",
    RrB: "231",
    image: gochizo_default,
  },
  A098: {
    name: "假面骑士Faiz",
    RrA: "215",
    RrB: "236",
    image: gochizo_default,
  },
  A099: {
    name: "假面骑士Blade",
    RrA: "215",
    RrB: "239",
    image: gochizo_default,
  },
  A100: {
    name: "假面骑士响鬼",
    RrA: "215",
    RrB: "242",
    image: gochizo_default,
  },
  A101: {
    name: "假面骑士Kabuto",
    RrA: "226",
    RrB: "97",
    image: gochizo_default,
  },
  A102: {
    name: "假面骑士Den-O",
    RrA: "226",
    RrB: "135",
    image: gochizo_default,
  },
  A103: {
    name: "假面骑士Kiva",
    RrA: "226",
    RrB: "156",
    image: gochizo_default,
  },
  A104: {
    name: "假面骑士Decade",
    RrA: "226",
    RrB: "181",
    image: gochizo_default,
  },
  A105: {
    name: "假面骑士W",
    RrA: "226",
    RrB: "196",
    image: gochizo_default,
  },
  A106: {
    name: "假面骑士OOO",
    RrA: "226",
    RrB: "206",
    image: gochizo_default,
  },
  A107: {
    name: "假面骑士Fource",
    RrA: "226",
    RrB: "215",
    image: gochizo_default,
  },
  A108: {
    name: "假面骑士Wizard",
    RrA: "226",
    RrB: "226",
    image: gochizo_default,
  },
  A109: {
    name: "假面骑士铠武",
    RrA: "226",
    RrB: "231",
    image: gochizo_default,
  },
  A110: {
    name: "假面骑士Drive",
    RrA: "226",
    RrB: "236",
    image: gochizo_default,
  },
  A111: {
    name: "假面骑士Ghost",
    RrA: "226",
    RrB: "239",
    image: gochizo_default,
  },
  A112: {
    name: "假面骑士EX-Aid",
    RrA: "226",
    RrB: "243",
    image: gochizo_default,
  },
  A113: {
    name: "假面骑士Build",
    RrA: "231",
    RrB: "97",
    image: gochizo_default,
  },
  A114: {
    name: "假面骑士Zio",
    RrA: "231",
    RrB: "135",
    image: gochizo_default,
  },
  A115: {
    name: "假面骑士01",
    RrA: "231",
    RrB: "156",
    image: gochizo_default,
  },
  A116: {
    name: "假面骑士Saber",
    RrA: "231",
    RrB: "181",
    image: gochizo_default,
  },
  A117: {
    name: "假面骑士Revi",
    RrA: "231",
    RrB: "196",
    image: gochizo_default,
  },
  A118: {
    name: "假面骑士Vice",
    RrA: "231",
    RrB: "206",
    image: gochizo_default,
  },
  A119: {
    name: "假面骑士极狐",
    RrA: "231",
    RrB: "215",
    image: gochizo_default,
  },
  A120: {
    name: "假面骑士太狸",
    RrA: "231",
    RrB: "226",
    image: gochizo_default,
  },
  A121: {
    name: "假面骑士霸牛",
    RrA: "231",
    RrB: "231",
    image: gochizo_default,
  },
  A122: {
    name: "假面骑士娜猫",
    RrA: "231",
    RrB: "236",
    image: gochizo_default,
  },
  A123: {
    name: "假面骑士歌查德",
    RrA: "231",
    RrB: "239",
    image: gochizo_default,
  },
  A124: {
    name: "假面骑士雷杰德",
    RrA: "231",
    RrB: "242",
    image: gochizo_default,
  },
  A125: {
    name: "假面骑士玛杰德",
    RrA: "236",
    RrB: "97",
    image: gochizo_default,
  },
  A126: {
    name: "假面骑士吉恩",
    RrA: "236",
    RrB: "135",
    image: gochizo_default,
  },
  A127: {
    name: "假面骑士瓦尔巴拉德",
    RrA: "236",
    RrB: "156",
    image: gochizo_default,
  },
  A128: {
    name: "假面骑士1号",
    RrA: "236",
    RrB: "181",
    image: gochizo_default,
  },
  A129: {
    name: "假面骑士巴尔坎",
    RrA: "236",
    RrB: "196",
    image: gochizo_default,
  },
  A130: {
    name: "假面骑士剑锋",
    RrA: "236",
    RrB: "206",
    image: gochizo_default,
  },
  A131: {
    name: "假面骑士莱伊布",
    RrA: "236",
    RrB: "215",
    image: gochizo_default,
  },
  A132: {
    name: "超级战队",
    RrA: "236",
    RrB: "226",
    image: gochizo_default,
  },
  A133: {
    name: "超级英雄",
    RrA: "236",
    RrB: "231",
    image: gochizo_default,
  },
  A134: {
    name: "奔奔者",
    RrA: "236",
    RrB: "236",
    image: gochizo_default,
  },
  A135: {
    name: "下午茶点心（咀嚼咀嚼）",
    RrA: "236",
    RrB: "239",
    image: gochizo_default,
  },
  A136: {
    name: "点心（咀嚼咀嚼）",
    RrA: "236",
    RrB: "242",
    image: gochizo_default,
  },
  A137: {
    name: "下午茶点心",
    RrA: "239",
    RrB: "97",
    image: gochizo_default,
  },
  A138: {
    name: "点心",
    RrA: "239",
    RrB: "135",
    image: gochizo_default,
  },
  A139: {
    name: "点心（咯嚓咯嚓）",
    RrA: "239",
    RrB: "156",
    image: gochizo_default,
  },
  A140: {
    name: "下午茶点心（咯嚓咯嚓）",
    RrA: "239",
    RrB: "181",
    image: gochizo_default,
  },
  A141: {
    name: "点心",
    RrA: "239",
    RrB: "196",
    image: gochizo_default,
  },
  A142: {
    name: "下午茶点心",
    RrA: "239",
    RrB: "206",
    image: gochizo_default,
  },
  A143: {
    name: "下午茶点心（多汁）",
    RrA: "239",
    RrB: "215",
    image: gochizo_default,
  },
  A144: {
    name: "点心（多汁）",
    RrA: "239",
    RrB: "226",
    image: gochizo_default,
  },
  A145: {
    name: "下午茶点心",
    RrA: "239",
    RrB: "231",
    image: gochizo_default,
  },
  A146: {
    name: "点心",
    RrA: "239",
    RrB: "236",
    image: gochizo_default,
  },
  A147: {
    name: "点心（舔一舔）",
    RrA: "239",
    RrB: "239",
    image: gochizo_default,
  },
  A148: {
    name: "下午茶点心（舔一舔）",
    RrA: "239",
    RrB: "242",
    image: gochizo_default,
  },
  A149: {
    name: "点心",
    RrA: "242",
    RrB: "97",
    image: gochizo_default,
  },
  A150: {
    name: "下午茶点心",
    RrA: "242",
    RrB: "135",
    image: gochizo_default,
  },
  A151: {
    name: "蛋糕王（惊异）",
    RrA: "242",
    RrB: "156",
    image: gochizo_default,
  },
  A152: {
    name: "圣诞树桩蛋糕（软绵绵）",
    RrA: "242",
    RrB: "181",
    image: gochizo_default,
  },
  A153: {
    name: "假面骑士加布",
    RrA: "242",
    RrB: "196",
    image: gochizo_default,
  },
  A154: {
    name: "假面骑士新骑士",
    RrA: "242",
    RrB: "206",
    image: gochizo_default,
  },
  A155: {
    name: "假面骑士新英雄",
    RrA: "242",
    RrB: "215",
    image: gochizo_default,
  },
  A156: {
    name: "假面骑士",
    RrA: "242",
    RrB: "226",
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
  var hexStr = "" + zeroPad(status, 2) + parseInt(RrA.value, 10).toString(16) + parseInt(RrB.value, 10).toString(16) + "00"
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
                  <input class="form-control" v-model="RrA" placeholder="226">

                  <h5 class="mb-0">RrB: {{ calcSPIToRes(RrB) }}k </h5>
                  <input class="form-control" v-model="RrB" placeholder="196">
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
