// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/FortBulletProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

AFortBulletProjectile::AFortBulletProjectile()
{
	 ProjectileMovementComp->InitialSpeed = 1000.f;
}
