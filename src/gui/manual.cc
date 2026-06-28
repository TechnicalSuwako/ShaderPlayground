/*
# 076 License

Copyright (c) 076 Studio LLC

Permission is hereby granted to any person obtaining a copy of the software
Shader Playground (the "Software") to use, modify, merge, copy, publish, distribute,
sublicense, and/or sell copies of the Software, subject to the following conditions:

    1. **Origin Attribution**:
       - You must not misrepresent the origin of the Software; you must not claim
         you created the original Software.
       - If the Software is used in a product, you must either:
         a. Provide clear attribution in the product's documentation, user interface,
            or other visible areas, **OR**
         b. Pay the original developers a fee they specify in writing.
    2. **Usage Restriction**:
       - The Software, or any derivative works, dependencies, or libraries
         incorporating it, must not be used for censorship or to suppress freedom of
         speech, expression, or creativity. Prohibited uses include, but are not
         limited to:
         - Censorship of so-called "hate speech", visuals, non-mainstream opinions,
           ideas, or objective reality.
         - Tools or systems designed to restrict access to information or
           artistic works.
    3. **Notice Preservation**:
       - This license and the above copyright notice must remain intact in all copies
         of the source code.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <gui/manual.hh>
#include <database/locale.hh>
#include <gui/guiengine.hh>
#include <imgui.h>
#include <imgui_markdown.h>

namespace gui {
  void Manual::Draw(Info &info) {
    if (!info.isManual) return;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowSize({ 600, 700 }, ImGuiCond_Appearing);
    ImGui::Begin(info.i18n->GetWord("helpmanual").c_str(), &info.isManual, flags);
    if (GuiEngine::ShouldCloseWindowOnEscape()) info.isManual = false;

    if (ImGui::BeginTabBar("ManualTabs")) {
      if (ImGui::BeginTabItem(info.i18n->GetWord("helpmanualoverview").c_str())) {
        m_CurPage = "index";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem(info.i18n->GetWord("helpmanualshader").c_str())) {
        m_CurPage = "shader";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem(info.i18n->GetWord("helpmanuallua").c_str())) {
        m_CurPage = "lua";
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem(info.i18n->GetWord("helpmanualhotkey").c_str())) {
        m_CurPage = "hotkeys";
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::Separator();

    ImGui::BeginChild("ManualContent", ImVec2(0, 0), 0, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::MarkdownConfig mdConfig = {};

    if (m_CurPage == "index") {
      string md;
      if (info.i18n->GetCurrentLanguage().code == "ja_JP") {
        ImGui::TextWrapped("Shader Playgroundへようこそ！");
        ImGui::Dummy(ImVec2(0, 10));
        md = R"(# はじめに

**Note**: For English, go to 編集 → 設定 (or press CTRL + ,), and set "言語/Language" to "English".

これはShader Playgroundのマニュアルです。
このマニュアルは、このプログラムをできるだけわかりやすく理解できるように作成されています。
ブラウザに切り替えてページの読み込みが遅くなるのを避けるため、マニュアルをプログラム内に埋め込むことにしました。
このテキストの上部には、このシステムの各部分の詳細に移動できる複数のタブがあります。
このタブは全体の簡単な概要を提供します。

# 基本的なUIガイド
初回起動時、ウィンドウが数秒間空白の画面を表示することがあります。
これはバックグラウンドでディレクトリを作成し、すべてのシーン データを保存するための新しいデータベース ファイルを作成しているためです。
次回起動時には、すべてが即座に読み込まれます。

起動直後に、ビューポートにはテクニカル諏訪子さんのGLSLチュートリアルで紹介されたシェーダーの1つが表示され、すべてが正常に動作していることを確認できます。
このシェーダーは、チュートリアルで使用された「GLSLテスター」プログラムのデフォルト シェーダーでもあります。
ビューポートのすぐ隣にはテキスト エディタのエリアがあります。
そこには3つのテキスト エディタがあります：頂点シェーダー用、フラグメントシェーダー用、そしてLua用です。
LuaはレンダリングのCPU側を制御し、シェーダーはGPU側を制御します。
最後に、下部にはログを出力するコンソールがあります。

その他の細かい詳細として、右上にFPSカウンターと日時が表示されます。
パフォーマンスを意識したり、寝る時間になったりするのに役立ちます。

新しいシェーダーを作成すると、いくつかの異なる開始点から選択できます。これらはLua APIのサンプルコードとしても機能します。
ビューポートはリアルタイム更新されません。
変更を確認するにはコンパイルが必要です。
これにより、フロー状態から外れることなくプログラミング/スクリプト作成ができます。

# 歴史
Shader Playgroundは、Shadertoyのようなプログラムをオフラインで使用でき、WebGLではなくOpenGLを使い、より大きな制御を可能にするという欲求から始まりました。
しかしLuaエンジンが実装されると、突然もっと有用なツールにしたいと思うようになりました。
そこで、ユーザーがGPU側だけでなくCPU側でもシェーダーを作成できるAPIを実装しました。
その後、3Dシェーダーを追加したくなり、気がつくと将来のリリースで拡張されるミニゲームエンジンになっていました。
目標は、移植性とパフォーマンスを犠牲にせずにできるだけ完全なものにすることですが、5日間のプロジェクトとしてそれらすべてを達成するのは現実的に不可能です。
そのため、将来のアップデートでゆっくりと確実にさらに多くの機能を追加していく予定です。)";
      } else {
        ImGui::TextWrapped("Welcome to Shader Playground!");
        ImGui::Dummy(ImVec2(0, 10));
        md = R"(# Intro

**注意**: 日本語で読むには、「Edit → Settings」をクリックし（若しくはCTRL + ,を押し）、「言語/Language」から「日本語」を御選択下さい。".

This is the manual for Shader Playground.
This manual aims to make it as easy to understand this program as possible.
We decided to embed the manual into the program to remove the need for switching to a web browser and suffer from slow page loading times.
At the top of this text you'll see multiple tabs going into details of each part of this system.
This tab gives a quick overview of everything.

# Basic UI Guide
Upon first start up, the window might show a blank screen for a few seconds.
This is because in the background, it's creating directories and a new database file to store all your scene data in.
Next time you run this program, everything will be instantly loaded.

Immediately upon opening, you'll see one of the shaders demonstrated in the GLSL tutorials by Technical Suwako in the viewport, confirming that it all works.
This shader is also the default in the program "GLSL Tester" which was used in those tutorials.
Right next to the viewport there is an area with a text editor.
It has 3 text editors: one for the vertex shader, one for the fragment shader, and one for Lua.
Lua controls the CPU side of the rendering, whereas the shaders control the GPU side.
Lastly, at the bottom there is the console that will output logs.

Some smaller details are the FPS counter and date/time at the right-top.
It allows for users to keep performance into account and also when it's finally time to sleep.

Upon creating a new shader, you'll get to choose from a couple of different starting points, which also doubles as a sample code for the Lua API.
The viewport does not update live.
Instead, it requires a compilation to see changes.
This allows for programming/scripting without getting kicked out of your flow state.

# History
Shader Playground started out as a desire to have a Shadertoy-like program, but that can be used offline, using OpenGL instead of WebGL, and also allow for greater control.
But as the Lua Engine got implemented, suddenly we wanted to make it into a much more useful tool.
So we implemented an API that lets the user make shaders not only on the GPU side, but on the CPU side as well.
Then we wanted to add in 3D shaders, and all of the sudden we had a mini game engine that will be expanded upon in future releases.
The aim is to make it as complete as possible without sacrificing on portability and performance, but as a 5 day project it's realistically not possible to accomplish all of that.
This is why future updates will slowly but surely add in more features.)";
      }
      ImGui::Markdown(md.c_str(), md.length(), mdConfig);
    } else if (m_CurPage == "shader") {
      string md;
      if (info.i18n->GetCurrentLanguage().code == "ja_JP") {
        md = R"(# シェーダー
現在、Shader PlaygroundはGLSLシェーダーのみをサポートしており、特にVertexシェーダーとFragmentシェーダーのみです。
シェーダーはShader PlaygroundのバックエンドであるOpenGL 4.6と互換性がある必要があります。
将来的にはVulkan GLSLシェーダー、NVN GLSLシェーダー（承認されたユーザーのみ）、Slangシェーダー、DirectX HLSLシェーダー（Windowsのみ）、Metalシェーダー（macOSのみ）もサポートされる予定です。
OpenGLのコアプロファイルがサポートするものはすべてShader Playgroundでサポートされており、実際のゲームからシェーダーをそのままコピーして使用できます。
GLSLエディタには抽象化が一切ないため、制約なく実世界のGLSLシェーダーで自由に実験できます。)";
      } else {
        md = R"(# Shaders
At the moment, Shader Playground only support GLSL shaders, and specifically only Vertex and Fragment shaders.
The shaders must be compatible with OpenGL 4.6, which is the backend of Shader Playground.
In the future it will be extended to also support Vulkan GLSL shaders, NVN GLSL shaders (only for approved users), Slang shaders, DirectX HLSL shaders (only on Windows), and Metal shaders (only on macOS).
Everything the core profile of OpenGL supports is supported in Shader Playground, and the shaders can be copied from real games as-is.
There are no abstractions for the GLSL editors, which provides full freedom to experiment with real world GLSL shaders without constraint.)";
      }
      ImGui::Markdown(md.c_str(), md.length(), mdConfig);
    } else if (m_CurPage == "lua") {
      string md;
      if (info.i18n->GetCurrentLanguage().code == "ja_JP") {
        md = R"(# Luaエンジン
Shader Playgroundは、基本的ながら強力なLuaスクリプトエンジンを提供しています。
独自の標準ライブラリが付属しており、`le.`で始まります。
このライブラリは、必要に応じてまだ開発中です。

## 関数
2つの関数があります：`Init`と`Update`。
`Init`関数はコンパイル後の最初のフレームでのみ実行されます。
`Update`関数は毎フレーム実行されます。

## コア
以下のAPI呼び出しは`le.`で始まります。
* `api_version` = Lua Engine APIのバージョンを返します。
* `log` = テキストと変数を出力します。Javascriptの`console.log()`と同様に動作します。任意のパラメータを受け取ります。
* `sizeof` = 各種データ型のサイズを返します。パラメータとして文字列でデータ型を受け取ります。

## システム
以下のAPI呼び出しは`le.sys`で始まります。
* `get_time` = Shader Playgroundが起動してからの総時間を返します。
* `get_delta_time` = `get_time`のデルタ値を返します。
* `get_timer_frequency` = Shader Playgroundのタイマー周波数を返します。
* `get_resolution` = Shader Playgroundの解像度（ビューポートではなく）を返します。
* `random` = 2つの整数間の乱数を返します。2つのパラメータ（両方文字列）を受け取ります。

## IO
以下のAPI呼び出しは`le.io`で始まります。
* `get_mouse` = マウスカーソルの位置を返します。
* `key.is_press` = 押されたキーを返します。文字列パラメータを受け取ります。
* `key.is_release` = 離されたキーを返します。文字列パラメータを受け取ります。
* `key.is_hold` = 押され続けているキーを返します。文字列パラメータを受け取ります。

## 数学
以下のAPI呼び出しは`le.math`で始まります。
* `identity` = 単位行列を返します。
* `translate` = 平行移動行列を返します。X, Y, Zの3つのfloatを受け取ります。
* `scale` = 拡大縮小行列を返します。X, Y, Zの3つのfloatを受け取ります。
* `rotate_x` = X軸回転行列を返します。角度の1つのfloatを受け取ります。
* `rotate_y` = Y軸回転行列を返します。角度の1つのfloatを受け取ります。
* `rotate_z` = Z軸回転行列を返します。角度の1つのfloatを受け取ります。
* `rotate` = 回転行列を返します。pitch, yaw, rollの3つのfloatを受け取ります。
* `perspective` = 透視投影行列を返します。視野角、アスペクト比、near、farの4つのfloatを受け取ります。
* `mul` = 2つの行列を掛けた結果を返します。2つの行列を受け取ります。
* `look_at` = LookAt行列を返します。eye, target, upそれぞれに3D軸の9つのfloatを受け取ります。

## グラフィックス
以下のAPI呼び出しは`le.gfx`で始まります。
* `add_mesh` = 頂点、インデックス、属性を受け取り、ビューポートに2Dまたは3Dオブジェクトを描画します。
* `get_position` = オブジェクトの現在の位置を返します。メッシュインデックスの1つの整数を受け取ります。
* `set_position` = メッシュの位置を変更します。メッシュインデックスの1つの整数とXYZ軸の3つのfloatを受け取ります。
* `get_rotation` = オブジェクトの現在の回転を返します。メッシュインデックスの1つの整数を受け取ります。
* `set_rotation` = メッシュの回転を変更します。メッシュインデックスの1つの整数とXYZ軸の3つのfloatを受け取ります。
* `get_scale` = オブジェクトの現在のスケールを返します。メッシュインデックスの1つの整数を受け取ります。
* `set_scale` = メッシュのスケールを変更します。メッシュインデックスの1つの整数とXYZ軸の3つのfloatを受け取ります。
* `set_uniform1` または `set_uniform_float1` = 1次元uniformを変更します。属性名の1つの文字列とX軸の1つのfloatを受け取ります。
* `set_uniform2` または `set_uniform_float2` = 2次元uniformを変更します。属性名の1つの文字列とXY軸の2つのfloatを受け取ります。
* `set_uniform3` または `set_uniform_float3` = 3次元uniformを変更します。属性名の1つの文字列とXYZ軸の3つのfloatを受け取ります。
* `set_uniform4` または `set_uniform_float4` = 4次元uniformを変更します。属性名の1つの文字列とXYZW軸の4つのfloatを受け取ります。
* `set_uniform_double1` = 1次元uniformを変更します。属性名の1つの文字列とX軸の1つのdoubleを受け取ります。
* `set_uniform_double2` = 2次元uniformを変更します。属性名の1つの文字列とXY軸の2つのdoubleを受け取ります。
* `set_uniform_double3` = 3次元uniformを変更します。属性名の1つの文字列とXYZ軸の3つのdoubleを受け取ります。
* `set_uniform_double4` = 4次元uniformを変更します。属性名の1つの文字列とXYZW軸の4つのdoubleを受け取ります。
* `set_uniform_int1` = 1次元uniformを変更します。属性名の1つの文字列とX軸の1つの符号付きintを受け取ります。
* `set_uniform_int2` = 2次元uniformを変更します。属性名の1つの文字列とXY軸の2つの符号付きintを受け取ります。
* `set_uniform_int3` = 3次元uniformを変更します。属性名の1つの文字列とXYZ軸の3つの符号付きintを受け取ります。
* `set_uniform_int4` = 4次元uniformを変更します。属性名の1つの文字列とXYZW軸の4つの符号付きintを受け取ります。
* `set_uniform_uint1` = 1次元uniformを変更します。属性名の1つの文字列とX軸の1つの符号なしintを受け取ります。
* `set_uniform_uint2` = 2次元uniformを変更します。属性名の1つの文字列とXY軸の2つの符号なしintを受け取ります。
* `set_uniform_uint3` = 3次元uniformを変更します。属性名の1つの文字列とXYZ軸の3つの符号なしintを受け取ります。
* `set_uniform_uint4` = 4次元uniformを変更します。属性名の1つの文字列とXYZW軸の4つの符号なしintを受け取ります。
* `set_uniform_vec3` = 3次元uniformを変更します。属性名の1つの文字列とXYZ軸の3つのfloatを受け取ります。
* `set_uniform_mat4` = 4x4行列uniformを変更します。属性名の1つの文字列と1つの4x4行列を受け取ります。

## キーコード
`key.is_press`、`key.is_release`、`key.is_hold`で使用できるキーコードは以下の通りです：
"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"
"a", "s", "d", "f", "g", "h", "j", "k", "l"
"z", "x", "c", "v", "b", "n", "m"
"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"
"enter", "space", "shift", "backspace"
"up", "down", "left", "right")";
      } else {
        md = R"(# Lua Engine
Shader Playground offers a basic, but powerful Lua scripting engine.
It comes with its own standard library, which starts with `le.`.
The library is still developing as needs grow.

## Functions
There are 2 functions: `Init` and `Update`.
The `Init` function executes only at the first frame from compile time.
The `Update` function executes on every frame.

## Core
The following API calls start with `le.`
* `api_version` = returns the version of Lua Engine API.
* `log` = prints text and variables, works similar to Javascript's `console.log()`. Takes in any parameters.
* `sizeof` = returns the size of various data types. Takes in data types as strings as parameter.

## System
The following API calls start with `le.sys`
* `get_time` = returns the total amount of time Shader Playground has been running.
* `get_delta_time` = returns the delta of `get_time`.
* `get_timer_frequency` = returns the timer frequency of Shader Playground.
* `get_resolution` = returns the resolution of Shader Playground, NOT the viewport.
* `random` = returns the random value between 2 integers. Takes in 2 parameters, both strings.

## IO
The following API calls start with `le.io`
* `get_mouse` = returns the mouse cursor position.
* `key.is_press` = returns the key pressed. Takes in string parameter.
* `key.is_release` = returns the key released. Takes in string parameter.
* `key.is_hold` = returns the key held. Takes in string parameter.

## Math
The following API calls start with `le.math`
* `identity` = returns the identity matrix.
* `translate` = returns the translation matrix. Takes in 3 floats for X, Y, and Z.
* `scale` = returns the scale matrix. Takes in 3 floats for X, Y, and Z.
* `rotate_x` = returns the rotation matrix on X. Takes in 1 float for angle.
* `rotate_y` = returns the rotation matrix on Y. Takes in 1 float for angle.
* `rotate_z` = returns the rotation matrix on Z. Takes in 1 float for angle.
* `rotate` = returns the rotation matrix. Takes in 3 floats for pitch, yaw, and roll.
* `perspective` = returns the perspective matrix. Takes in 4 floats for field of view, aspect, near, and far.
* `mul` = returns the result of 2 matrices multiplied together. Takes in 2 matrices.
* `look_at` = returns the look at matrix. Takes in 9 floats for eye, target, and up of each 3D axis.

## Graphics
The following API calls start with `le.gfx`
* `add_mesh` = takes in vertices, indices, and attributes to then render either a 2D or 3D object in the viewport.
* `get_position` = returns the current position of an object. Takes in 1 integer for the mesh index.
* `set_position` = modifies the position of a mesh. Takes in 1 integer for the mesh index and 3 floats for the XYZ axis.
* `get_rotation` = returns the current rotation of an object. Takes in 1 integer for the mesh index.
* `set_rotation` = modifies the rotation of a mesh. Takes in 1 integer for the mesh index and 3 floats for the XYZ axis.
* `get_scale` = returns the current scale of an object. Takes in 1 integer for the mesh index.
* `set_scale` = modifies the scale of a mesh. Takes in 1 integer for the mesh index and 3 floats for the XYZ axis.
* `set_uniform1` or `set_uniform_float1` = modifies a 1D uniform. Takes in 1 string for the attribute name and 1 float for the X axis.
* `set_uniform2` or `set_uniform_float2` = modifies a 2D uniform. Takes in 1 string for the attribute name and 2 floats for the XY axis.
* `set_uniform3` or `set_uniform_float3` = modifies a 2D uniform. Takes in 1 string for the attribute name and 3 floats for the XYZ axis.
* `set_uniform4` or `set_uniform_float4` = modifies a 2D uniform. Takes in 1 string for the attribute name and 4 floats for the XYZW axis.
* `set_uniform_double1` = modifies a 1D uniform. Takes in 1 string for the attribute name and 1 double for the X axis.
* `set_uniform_double2` = modifies a 2D uniform. Takes in 1 string for the attribute name and 2 doubles for the XY axis.
* `set_uniform_double3` = modifies a 2D uniform. Takes in 1 string for the attribute name and 3 doubles for the XYZ axis.
* `set_uniform_double4` = modifies a 2D uniform. Takes in 1 string for the attribute name and 4 doubles for the XYZW axis.
* `set_uniform_int1` = modifies a 1D uniform. Takes in 1 string for the attribute name and 1 signed int for the X axis.
* `set_uniform_int2` = modifies a 2D uniform. Takes in 1 string for the attribute name and 2 signed ints for the XY axis.
* `set_uniform_int3` = modifies a 2D uniform. Takes in 1 string for the attribute name and 3 signed ints for the XYZ axis.
* `set_uniform_int4` = modifies a 2D uniform. Takes in 1 string for the attribute name and 4 signed ints for the XYZW axis.
* `set_uniform_uint1` = modifies a 1D uniform. Takes in 1 string for the attribute name and 1 unsigned int for the X axis.
* `set_uniform_uint2` = modifies a 2D uniform. Takes in 1 string for the attribute name and 2 unsigned ints for the XY axis.
* `set_uniform_uint3` = modifies a 2D uniform. Takes in 1 string for the attribute name and 3 unsigned ints for the XYZ axis.
* `set_uniform_uint4` = modifies a 2D uniform. Takes in 1 string for the attribute name and 4 unsigned ints for the XYZW axis.
* `set_uniform_vec3` = modifies a 3D uniform. Takes in 1 string for the attribute name and 3 floats for the XYZ axis.
* `set_uniform_mat4` = modifies a 4D uniform. Takes in 1 string for the attribute name and 1 4x4 matrix.

## Key codes
The following key codes are supported for use with `key.is_press`, `key.is_release`, and `key.is_hold`:
"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"
"a", "s", "d", "f", "g", "h", "j", "k", "l"
"z", "x", "c", "v", "b", "n", "m"
"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"
"enter", "space", "shift", "backspace"
"up", "down", "left", "right")";
      }
      ImGui::Markdown(md.c_str(), md.length(), mdConfig);
      ImGui::Markdown(md.c_str(), md.length(), mdConfig);
    } else if (m_CurPage == "hotkeys") {
      string md;
      if (info.i18n->GetCurrentLanguage().code == "ja_JP") {
        md = R"(# ホットキー
Shader Playgroundは、ほとんどの操作にホットキーを提供しています。

## UI
CTRL + Tab = ウィンドウの切り替え
ビューポート上で右クリック = クリア色の変更
F1 = このマニュアルを開く
CTRL + , = 設定ウィンドウを開く
CTRL + H = 「Shader Playgroundについて」ウィンドウを開く
Escape = 現在のポップアップウィンドウおよび/またはモーダルを閉じる

## シーン
CTRL + N = 新しいシーンを作成
CTRL + O = 既存のシーンを開く
CTRL + S = 現在のシーンを保存
CTRL + Shift + S = 現在のシーンを別名で保存

## ビルド
F5 = シェーダーとLuaコードをコンパイル

## エディタ
CTRL + Z = 元に戻す
CTRL + Y = やり直し
CTRL + X = 切り取り
CTRL + C = コピー
CTRL + V = 貼り付け
CTRL + D = 複製
CTRL + A = 全て選択)";
      } else {
        md = R"(# Hotkeys
Shader Playground offers hotkeys for most operations.

## UI
CTRL + Tab = switch between windows
Right click on viewport = change clear color
F1 = open this manual
CTRL + , = open settings window
CTRL + H = open "About Shader Playground" window
Escape = close current popup window and/or modal

## Scenes
CTRL + N = create new scene
CTRL + O = open an already existing scene
CTRL + S = save current scene
CTRL + Shift + S = save current scene under a different name

## Building
F5 = compile shaders and Lua code

## Editor
CTRL + Z = undo
CTRL + Y = redo
CTRL + X = cut
CTRL + C = copy
CTRL + V = paste
CTRL + D = duplicate
CTRL + A = select all)";
      }
      ImGui::Markdown(md.c_str(), md.length(), mdConfig);
    }

    ImGui::EndChild();
    ImGui::End();
  }

  /////////

  void Manual::renderMarkdown(cstr text) {
    ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
    ImGui::TextUnformatted(text);
    ImGui::PopTextWrapPos();
  }
} // namespace gui