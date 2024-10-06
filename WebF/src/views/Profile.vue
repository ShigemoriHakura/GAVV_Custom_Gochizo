<script setup>
import { onBeforeMount, onMounted, onBeforeUnmount } from "vue";
import ArgonButton from "@/components/ArgonButton.vue";
import { useStore } from "vuex";

import setNavPills from "@/assets/js/nav-pills.js";
import setTooltip from "@/assets/js/tooltip.js";

const body = document.getElementsByTagName("body")[0];

const store = useStore();

onMounted(() => {
  store.state.isAbsolute = true;
  setNavPills();
  setTooltip();
});
onBeforeMount(() => {
  store.state.imageLayout = "profile-overview";
  store.state.showNavbar = false;
  store.state.showFooter = true;
  store.state.hideConfigButton = true;
  body.classList.add("profile-overview");
});
onBeforeUnmount(() => {
  store.state.isAbsolute = false;
  store.state.imageLayout = "default";
  store.state.showNavbar = true;
  store.state.showFooter = true;
  store.state.hideConfigButton = false;
  body.classList.remove("profile-overview");
});
</script>
<template>
  <main>
    <div class="container-fluid">
      <div
        class="page-header min-height-300"
        style="
          background-image: url(&quot;https://images.unsplash.com/photo-1531512073830-ba890ca4eba2?ixid=MnwxMjA3fDB8MHxwaG90by1wYWdlfHx8fGVufDB8fHx8&ixlib=rb-1.2.1&auto=format&fit=crop&w=1920&q=80&quot;);
          margin-right: -24px;
          margin-left: -34%;
        "
      >
        <span class="mask bg-gradient-success opacity-6"></span>
      </div>
      <div class="card shadow-lg mt-n6">
        <div class="card-body p-3">
          <div class="row gx-4">
            <div class="col-auto">
              <div class="avatar avatar-xl position-relative">
                <img
                  src="../assets/img/Logo.png"
                  alt="profile_image"
                  class="shadow-sm w-100 border-radius-lg"
                />
              </div>
            </div>
            <div class="col-auto my-auto">
              <div class="h-100">
                <h5 class="mb-1">万能GAVV - 网页配置器</h5>
                <p class="mb-0 font-weight-bold text-sm">开源让世界更美好？</p>
              </div>
            </div>
            <br>
            <br>
            <h5>
              本项目完全开源，感谢你的使用。
              <br>
              项目地址：
              <br>
              https://github.com/ShigemoriHakura/GAVV_Custom_Gochizo
              <br>
              
              喜欢本项目的话，记得给我一个一键三连和关注，谢谢喵！
              <br>
              <br>

              <a href="https://space.bilibili.com/1930322">
                <argon-button full-width>B站链接</argon-button></a>
                <br>
                <br>
              <a href="https://github.com/ShigemoriHakura/GAVV_Custom_Gochizo">
                <argon-button full-width>项目地址</argon-button></a>
              
            </h5>
          </div>
        </div>
      </div>
    </div>
  </main>
</template>
