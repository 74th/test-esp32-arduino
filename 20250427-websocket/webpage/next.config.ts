import type { NextConfig } from "next";

const nextConfig: NextConfig = {
  allowedDevOrigins: [
    "http://192.168.1.0/24", // ローカルネットワーク全体を許可（必要に応じて調整）
    "http://localhost:3000",
  ],
};

export default nextConfig;
