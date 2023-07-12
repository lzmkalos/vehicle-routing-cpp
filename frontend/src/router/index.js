import { createRouter, createWebHistory } from "vue-router";

import HomeView from "@/views/HomeView.vue";
import PointsView from "@/views/PointsView.vue";

const routes = [
  {
    path: "/",
    name: "home",
    component: HomeView,
  },
  {
    path: "/points",
    name: "points",
    component: PointsView,
  },
];

const router = createRouter({
    history: createWebHistory(),
    routes,
});

export default router;