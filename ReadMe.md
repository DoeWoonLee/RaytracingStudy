# RayTracing Note

[TOC]

## Command

- Start or Stop Rendering : Ctrl + S

## About Release or Delete

- 메모리 관리는 원래는 MemoryPool에 의해서 관리되게 할 예정
- 메모리풀 문제로 new로인한 메모리 할당을 하고 있고, 해제 코드가 들어있다.
	- MemoryPool의 #define MEMORYPOOLUSE 로 관리
- 나중에 해제기능을 지원하는 메모리 풀을 만들었을 경우 결국엔 해제코드가 들어가야한다.


## FieldObject Class

- 실질적으로 그려질 오브젝트들의 묶음을 담당한다.
  - 오브젝트의 특성은 `Component`들의 묶음으로 만들어진다.
- Local -> World , World -> Local의 변환을 조절한다.
- Material, Resource, Transform, AABB를 사용한다. 
	- 이 데이터들은 동적할당으로 얻는 데이터이다.
	- Resource만 중복으로 사용하고 `RefCnt`를 가지며, `SAFE_RELEASE()`함수를 사용한다.

## BaseClass [ SuperClass ]
- `RefCnt`를 관리하는 클래스이다.
- 이 클래스를 상속 받으면 소멸자는 `protected: virtual ~Deconstructor()`형태이다.
- `SAFE_RELEASE()`로 해제해야한다.


# Component

- `FieldObject`에 의해 소유 되며 한 Container에 추상되기 위해서 존재한다.
- 아직 사용하지 않음

## Transform Class [ Component ]

- 행렬을 사용함
  - World, InversWorld두개를 미리 계산해둠
    - InverseWorld를 미리 계산해둔 이유는 광선을 로컬영역으로 보내기 위해서
  - `Vector3`에 대해 Local -> World, World -> Local 함수지원

## Material Class [ Component ]

- 각종 재질의 추상화를 위한 추상클래스

- 순수가상함수는 `Scatter()`와 `Emiited()`가 있다.

  - `Scatter()`는 광선이 물체에 충돌 했을때, 어떠한 색의 감쇄가 일어 나는지, 빛의 다음 이동방향을 결정짓는다.
  - `Emitted()`는 광선이 물체에 충돌 했을때 스스로 빛을 얼마나 발하는가를 결정하는 함수이다. 결과는 Return으로 얻음.

- `Texture`를 받아서 해당 Texture의 UV좌표로 색을 입힌다. (아직 추가되어지지 않음)

## AABB Class 

- `Resource`의 AABB데이터를 `Transform`의 월드 행렬과 연산하여 월드기준 `AABB`를 얻는다.
- `FieldObject`가 소유하며 `Component`로 취급할지는 미정 [ 2020-01-20 ]

# Material 

## Lambertain Class [ Material ]

- 표면이 거친정도가 있는 일반적인 물체를 말함.
- OutRay = Normalize(Normal + RandomUnitSphereVector)


## Metal Class [ Material ]

- 광선의 입사각과 반사각이 동일한 물체
- OutRay = InRay.Dir - cosf(InRay.Dir, Normal) * 2.f
  - -인 이유는 Cos의 결과가 반대이기 때문
- Fuzz인자를 선택적으로 받을 수 있으며, 금속의 거칠기를 표현함
  

## Dielectric Class [ Material ]

- 광선이 물체를 투과하는 재질을 말함.
- 스넬의 법칙이 적용되며, 생성시 인자로는 Refract Index를 받음

## DiffuseLight Class [ Material ]

- 빛을 내는 물체를 표현
- `Emmit()`함수로 표현한다.

## Isotropic Class [ Material ]


# Texture

## ConstantTexture [ Texture ]
- 단일 색상을 표시하는 Texture
- UV값이 필요 없다.


## Resources Class [ Component ]

- 물체의 모양을 결정하며, 광선의 충돌의 실질적인 연산이 이루어지는 곳
- 가상함수는 `MakeMinMax()` , `Hit()`이다. `Hit()`은 순수 가상함수이다.
  - `MakeMinMax()`를 가상함수로 만든 이유는 수식 물체들은 코드상에서 지정해 주는 것이 좋다. (ex Sphere, Cylinder...)
  - `Hit()`함수는 `Ray`클래스를 인자로 받으며 이 클래스는 `Resources`의 로컬상의 좌표계로 이미 이동되어진 광선이다.
  - `Hit()`함수는 FieldObject단에서 AABB충돌을 한번처리하고 만약 `Mesh`라면 `KDTree`연산을 거친후 `Triangle`연산을 한다.

## Mesh Class [ Resources ]

