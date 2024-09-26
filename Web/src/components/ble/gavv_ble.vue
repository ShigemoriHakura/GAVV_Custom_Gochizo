<template>
    <div class=app>
        <div>
            <Button @click=onClick class=blue>Click to start</Button>
        </div>
    </div>
</template>

<script>
import Button from 'primevue/button';

const SERVICE = "3426d525-b6e9-4489-80f4-632e2251a2a6"
const STATUS = "3426d545-b6e9-4489-80f4-632e2251a2a6"
const RRA = "3426d550-b6e9-4489-80f4-632e2251a2a6"
const RRB = "3426d551-b6e9-4489-80f4-632e2251a2a6"
const ACTION = "3426d551-b6e9-4489-80f4-632e2251a2a6"

export default {
    name: 'app',
    components: {
        Button
    },
    data: () => ({

    }),
    methods: {
        async onClick() {

            console.log('loading')
            navigator.bluetooth.requestDevice({
                filters: [{ services: [SERVICE] }],
            })
            .then(async device => {
                console.log('Name: ' + device.name)
                console.log(device)
                device.addEventListener('gattserverdisconnected', this.onDisconnected);
                await device.gatt.connect()
                console.log(device.gatt)
                return device.gatt.connect()
            })
            .then(server => {
                console.log("server")
                console.log(server)
                return server.getPrimaryService(SERVICE)
            })
            .then(service => {
                console.log("service")
                console.log(service)
                return Promise.all([
                    service.getCharacteristic(STATUS),
                    service.getCharacteristic(RRA),
                    service.getCharacteristic(RRB),
                    service.getCharacteristic(ACTION),
                ])
            })
            .then(async characteristics => {
                console.log(characteristics)
                //return characteristic.readValue();
            })
            //.then(characteristic => characteristic.startNotifications())
            //.then(characteristic => characteristic.addEventListener('characteristicvaluechanged', this.handleCharacteristicValueChanged.bind(this)))
            .catch(error => console.log(error));
        },
        onDisconnected() {
            console.log('Device is disconnected.');
        }
    },
};
</script>