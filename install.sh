#!/usr/bin/env bash
# install.sh - Convenience installer for Debian/Ubuntu systems to build and install the GTK desktop shell

set -euo pipefail

APP_NAME="hybrid-desktop"
PREFIX="/usr/local"
DATADIR="$PREFIX/share/$APP_NAME"
DESKTOP_FILE="org.example.hybriddesktop.desktop"

warn_os() {
    if [[ -r /etc/os-release ]]; then
        if ! grep -qiE 'debian|ubuntu' /etc/os-release; then
            echo "[!] このスクリプトは Debian/Ubuntu 系を想定していますが、続行します。" >&2
        else
            echo "[*] Debian/Ubuntu 系 OS を検出しました。"
        fi
    else
        echo "[!] /etc/os-release が見つかりません。環境によっては動作しない可能性があります。" >&2
    fi
}

prompt_update() {
    read -r -p "sudo apt update を実行しますか？ [y/N]: " ans
    if [[ "$ans" =~ ^[Yy]$ ]]; then
        echo "[*] sudo apt update を実行します..."
        if ! sudo apt update; then
            echo "[!] apt update に失敗しました。" >&2
            exit 1
        fi
    else
        echo "[!] apt update をスキップしました。パッケージ情報が古い場合、インストールに失敗することがあります。"
    fi
}

install_packages() {
    echo "[*] 依存パッケージをインストールします: build-essential libgtk-3-dev pkg-config"
    if ! sudo apt install -y build-essential libgtk-3-dev pkg-config; then
        echo "[!] パッケージインストールに失敗しました。" >&2
        exit 1
    fi
}

build_project() {
    echo "[*] プロジェクトをビルドします (make) ..."
    if ! make; then
        echo "[!] make に失敗しました。" >&2
        exit 1
    fi
}

install_project() {
    echo "[*] システムにインストールします (sudo make install) ..."
    if ! sudo make install; then
        echo "[!] make install に失敗しました。" >&2
        exit 1
    fi
}

main() {
    echo "=== LinuxGUI セットアップを開始します ==="
    warn_os
    prompt_update
    install_packages
    build_project
    install_project
    echo "=== インストール完了! ==="
    echo "- バイナリ: $PREFIX/bin/${APP_NAME}"
    echo "- リソース: $DATADIR"
    echo "- デスクトップエントリ: /usr/share/applications/${DESKTOP_FILE}"
    echo "起動方法: '${APP_NAME}' コマンド、またはデスクトップメニューから起動できます。"
    echo "自動起動したい場合は ${DESKTOP_FILE} を ~/.config/autostart/ にコピーしてください。"
}

main "$@"
