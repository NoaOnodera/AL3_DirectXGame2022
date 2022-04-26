﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	// delete sprite_;
	delete model_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	textureHandle_ = TextureManager::Load("mario.jpg"); //ファイル名を指定してテクスチャを読み込む
	sprite_ = Sprite::Create(textureHandle_, {100, 0}); //スプライトの生成
	model_ = Model::Create();                           // 3Dモデルの生成
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	//std::uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	//std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	

		worldTransform_[PartId::Root].scale_ = {0.6f, 1.0f, 1.0f};
	
		
	

	worldTransform_[PartId::Root].Initialize();

	//脊髄
	
	worldTransform_[PartId::Spine].translation_ = {0.0f,0.0f, 0.0f};
	worldTransform_[PartId::Spine].parent_ = &worldTransform_[PartId::Root];
	worldTransform_[PartId::Spine].Initialize();
	
	//上半身
	worldTransform_[Chest].translation_ = {0.0f, 5.0f, 0.0};
	worldTransform_[Chest].parent_ = &worldTransform_[Spine];
	worldTransform_[Chest].Initialize();

   
	worldTransform_[Head].translation_ = {0.0f, 3.0f, 0.0f};
	worldTransform_[Head].parent_ = &worldTransform_[Chest];
	worldTransform_[Head].Initialize();

	worldTransform_[ArmL].translation_ = {-3.0f, 0.0f, 0.0f};
	worldTransform_[ArmL].parent_ = &worldTransform_[Chest];
	worldTransform_[ArmL].Initialize();

	worldTransform_[ArmR].translation_ = {3.0f, 0.0f, 0.0f};
	worldTransform_[ArmR].parent_ = &worldTransform_[Chest];
	worldTransform_[ArmR].Initialize();


	//下半身
	worldTransform_[Hip].translation_ = {0.0f, 2.0f, 0.0f};
	worldTransform_[Hip].parent_ = &worldTransform_[Spine];
	worldTransform_[Hip].Initialize();

	worldTransform_[LegL].translation_ = {-3.0f, -3.0f, 0.0f};
	worldTransform_[LegL].parent_ = &worldTransform_[Hip];
	worldTransform_[LegL].Initialize();

	worldTransform_[LegR].translation_ = {3.0f, -3.0f, 0.0f};
	worldTransform_[LegR].parent_ = &worldTransform_[Hip];
	worldTransform_[LegR].Initialize();


	viewProjection_.up = {0.0f, 1.0f, 0.0f};
	viewProjection_.target = {0, 0, 0};
	viewProjection_.eye = {0.0f,0.0f,-50};



	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = XMConvertToRadians(45.0f);
	//アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;
	////ニアクリップ距離を設定
	viewProjection_.nearZ = 0.10f;
	////ファークリップ距離を設定
	viewProjection_ .farZ = 53.0f;

	viewProjection_.Initialize(); //ビュープロジェクションの初期化
}

void GameScene::Update() {

	XMFLOAT3 move = {0, 0, 0};

	const float kCharacterSpeed  =0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move = {-kCharacterSpeed, 0, 0};

	} else if (input_->PushKey(DIK_RIGHT)) {
		move = {kCharacterSpeed, 0, 0};
	}

	//上半身の回転速さ[ラジアン/frame]
	const float kChestRotSpeed = 0.05f;

	//押した方向で移動べくトルを変更
	if (input_->PushKey(DIK_U)) {
		worldTransform_[PartId::Chest].rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_I)) {
		worldTransform_[PartId::Chest].rotation_.y += kChestRotSpeed;
	}

	const float kHipRotSpeed = 0.05;

	if (input_->PushKey(DIK_J)) {
		worldTransform_[PartId::Hip].rotation_.y -= kHipRotSpeed;
	} else if (input_->PushKey(DIK_K)) {
		worldTransform_[PartId::Hip].rotation_.y += kHipRotSpeed;
	}


	worldTransform_[PartId::Root].translation_.x += move.x;
	worldTransform_[PartId::Root].translation_.y += move.y;
	worldTransform_[PartId::Root].translation_.z += move.z;


	////デバッグ用表示
	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "Root::(%f,%f,%f", worldTransform_[PartId::Root].translation_.x,
	worldTransform_[PartId::Root].translation_.y, worldTransform_[PartId::Root].translation_.z);

	 debugText_->SetPos(50, 50);
	debugText_->Printf(
	  "eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf(
	  "target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
	  viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
	  "up:(% f, % f,% f) ", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

	
	 debugText_->SetPos(50, 110);
	 debugText_->Printf("fovAngleY(Degree):%f", XMConvertToDegrees(viewProjection_.fovAngleY));

	  debugText_->SetPos(50, 130);
	 debugText_->Printf("nearZ:%f", XMConvertToDegrees(viewProjection_.nearZ));


	worldTransform_[PartId::Root].UpdateMatrix();
	worldTransform_[PartId::Spine].UpdateMatrix();

	worldTransform_[PartId::Chest].UpdateMatrix();
	worldTransform_[PartId::Head].UpdateMatrix();
	worldTransform_[PartId::ArmL].UpdateMatrix();
	worldTransform_[PartId::ArmR].UpdateMatrix();
	worldTransform_[PartId::Hip].UpdateMatrix();
	worldTransform_[PartId::LegL].UpdateMatrix();
	worldTransform_[PartId::LegR].UpdateMatrix();
	

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデルの描画
	for (size_t i = 0; i < _countof(worldTransform_); i++) {

		//model_->Draw(worldTransform_[0], viewProjection_, textureHandle_);
		//model_->Draw(worldTransform_[1], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::Chest], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::Head], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::ArmL], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::ArmR], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::Hip], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::LegL], viewProjection_, textureHandle_);
		model_->Draw(worldTransform_[PartId::LegR], viewProjection_, textureHandle_);

	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
