void DrawString(int x, int y, DWORD color, const char* fmt, DWORD TRPE = DT_NOCLIP)
{
	RECT FontPos = { x, y, x + 120, y + 20 };
	draw.font->DrawTextA(NULL, fmt, -1, &FontPos, TRPE, color);
}

void DrawRect(float x, float y, float w, float h, D3DCOLOR color, IDirect3DDevice9Ex* pDevice)
{
	D3DRECT rec = { x, y, x + w, y + h };
	pDevice->Clear(1, &rec, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawBorder(float x, float y, float w, float h, float px, D3DCOLOR BorderColor, IDirect3DDevice9Ex* pDevice)
{
	DrawRect(x, (y + h - px), w, px, BorderColor, pDevice);
	DrawRect(x, y, px, h, BorderColor, pDevice);
	DrawRect(x, y, w, px, BorderColor, pDevice);
	DrawRect((x + w - px), y, px, h, BorderColor, pDevice);
}
void DrawCornerBox(float x, float y, float w, float h, float borderPx, D3DCOLOR color, IDirect3DDevice9Ex* pDev)
{
	// draw.GetDevice
	DrawRect(x + borderPx, y, w / 3, borderPx, color, pDev); //top 
	DrawRect(x + w - w / 3 + borderPx, y, w / 3, borderPx, color, pDev); //top 
	DrawRect(x, y, borderPx, h / 3, color, pDev); //left 
	DrawRect(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color, pDev); //left 
	DrawRect(x + borderPx, y + h + borderPx, w / 3, borderPx, color, pDev); //bottom 
	DrawRect(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx, color, pDev); //bottom 
	DrawRect(x + w + borderPx, y, borderPx, h / 3, color, pDev);//right 
	DrawRect(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color, pDev);//right 
}

void DrawBox(int x, int y, int w, int h, D3DCOLOR BoxColor, D3DCOLOR BorderColor, IDirect3DDevice9Ex* pDevice)
{
	DrawRect(x, y, w, h, BoxColor, pDevice);
	DrawBorder(x, y, w, h, 1, BorderColor, pDevice);
}

void DrawESPBox(Vector3 Head, Vector3 Foot, DWORD dwColor, int recAlpha)
{
	Vector3 Box = Head - Foot;

	if (Box.y < 0)
		Box.y *= -1;

	int BoxWidth = (int)Box.y / 2.4f;
	int DrawX = (int)Head.x - (BoxWidth / 2);
	int DrawY = (int)Head.y;

	DrawBox(DrawX, DrawY, BoxWidth + 1, (int)Box.y + 5, D3DCOLOR_ARGB(10, 10, 0, 0), dwColor, draw.GetDevice());
}

LPD3DXLINE xwwwwwwqq = NULL;
void DrawLine(float StartX, float StartY, float EndX, float EndY, D3DCOLOR dColor, float thicc)
{
	xwwwwwwqq[0].SetWidth(thicc);
	xwwwwwwqq[0].SetGLLines(0);
	xwwwwwwqq[0].SetAntialias(1);
	D3DXVECTOR2 v2Line[2];
	v2Line[0].x = StartX;
	v2Line[0].y = StartY;
	v2Line[1].x = EndX;
	v2Line[1].y = EndY;

	xwwwwwwqq[0].Begin();
	xwwwwwwqq[0].Draw(v2Line, 2, dColor);
	xwwwwwwqq[0].End();
}

static void box3d_lines(Vector3 center, float x1, float y1, float z1, float x2, float y2, float z2, DWORD col)
{
	Vector3 pointPos1 = { center.x + x1, center.y + y1, center.z + z1 };
	Vector3 pointPos2 = { center.x + x2, center.y + y2, center.z + z2 };

	Vector3 sxy1 = WorldToScreen(pointPos1);
	Vector3 sxy2 = WorldToScreen(pointPos2);

	if (sxy1.x != 0 || sxy1.y != 0 || sxy2.x != 0 || sxy2.y != 0)
		DrawLine(sxy1.x, sxy1.y, sxy2.x, sxy2.y, col, 1.3);
}

static void box3d(Vector3 center, float w, float h, DWORD col)
{
#define padding 10
	box3d_lines(center, -w, -w, 0, w, -w, 0, col);
	box3d_lines(center, -w, -w, h, w, -w, h, col);

	box3d_lines(center, -w, -w, 0, -w, -w, h, col);
	box3d_lines(center, w, -w, 0, w, -w, h, col);

	box3d_lines(center, w, w + padding, 0, w, w + padding, h, col);
	box3d_lines(center, -w, w + padding, 0, -w, w + padding, h, col);
	box3d_lines(center, w, w + padding, 0, -w, w + padding, 0, col);
	box3d_lines(center, w, w + padding, h, -w, w + padding, h, col);

	box3d_lines(center, -w, -w, 0, -w, w + padding, 0, col);
	box3d_lines(center, w, w + padding, 0, w, -w, 0, col);
	box3d_lines(center, -w, -w, h, -w, w + padding, h, col);
	box3d_lines(center, w, w + padding, h, w, -w, h, col);
}

void DrawSkeleton(uintptr_t Mesh, bool IsMale, DWORD col)
{
	int skeleton[][4] = {
		{ 7, 8, 0, 0},
		{ 7, IsMale ? 47 : 46, IsMale ? 48 : 47, IsMale ? 49 : 48 },
		{ 7, 21, 22, 23 },
		{ 7, 5, 4, 3 },
		{ 3, IsMale ? 82 : 80, IsMale ? 83 : 81, IsMale ? 85 : 83 },
		{ 3, IsMale ? 75 : 73, IsMale ? 76 : 74, IsMale ? 78 : 76 }
	};

	for (auto part : skeleton)
	{
		Vector3 previous{};
		for (int i = 0; i < 4; i++)
		{
			if (!part[i]) break;

			Vector3 current = GetBoneWithRotation(Mesh, part[i]);
			if (previous.x == 0.f)
			{
				previous = current;
				continue;
			}
			Vector3 p1{}, c1{};
			p1 = WorldToScreen(previous);
			c1 = WorldToScreen(current);
			DrawLine(p1.x, p1.y, c1.x, c1.y, col, 1.5f);
			previous = current;
		}
	}
}

Vector3 FindSkeleton(DWORD_PTR mesh, int i)
{
	return WorldToScreen(GetBoneWithRotation(mesh, i));
}

void DrawSkeletonBot(DWORD_PTR mesh, DWORD cColor, float kalinlik = 2.0f)
{

	Vector3 vHipOut = FindSkeleton(mesh, 3);

	Vector3 vHeadBoneOut = FindSkeleton(mesh, 8);
	Vector3 vNeckOut = FindSkeleton(mesh, 7);

	Vector3 vUpperArmLeftOut = FindSkeleton(mesh, 11);
	Vector3 vLeftHandOut = FindSkeleton(mesh, 12);
	Vector3 vLeftHandOut1 = FindSkeleton(mesh, 13);

	Vector3 vUpperArmRightOut = FindSkeleton(mesh, 36);
	Vector3 vRightHandOut = FindSkeleton(mesh, 37);
	Vector3 vRightHandOut1 = FindSkeleton(mesh, 38);



	Vector3 vLeftThighOut = FindSkeleton(mesh, 63);
	Vector3 vLeftCalfOut = FindSkeleton(mesh, 65);
	Vector3 vLeftFootOut = FindSkeleton(mesh, 69);

	Vector3 vRightThighOut = FindSkeleton(mesh, 77);
	Vector3 vRightCalfOut = FindSkeleton(mesh, 79);
	Vector3 vRightFootOut = FindSkeleton(mesh, 83);
	// if (distance > 0)
	{
		DrawLine(vNeckOut.x, vNeckOut.y, vHeadBoneOut.x, vHeadBoneOut.y, cColor, kalinlik);
		DrawLine(vHipOut.x, vHipOut.y, vNeckOut.x, vNeckOut.y, cColor, kalinlik);
		DrawLine(vUpperArmLeftOut.x, vUpperArmLeftOut.y, vNeckOut.x, vNeckOut.y, cColor, kalinlik);
		DrawLine(vUpperArmRightOut.x, vUpperArmRightOut.y, vNeckOut.x, vNeckOut.y, cColor, kalinlik);
		DrawLine(vLeftHandOut.x, vLeftHandOut.y, vUpperArmLeftOut.x, vUpperArmLeftOut.y, cColor, kalinlik);
		DrawLine(vRightHandOut.x, vRightHandOut.y, vUpperArmRightOut.x, vUpperArmRightOut.y, cColor, kalinlik);
		DrawLine(vLeftHandOut.x, vLeftHandOut.y, vLeftHandOut1.x, vLeftHandOut1.y, cColor, kalinlik);
		DrawLine(vRightHandOut.x, vRightHandOut.y, vRightHandOut1.x, vRightHandOut1.y, cColor, kalinlik);
		DrawLine(vLeftThighOut.x, vLeftThighOut.y, vHipOut.x, vHipOut.y, cColor, kalinlik);
		DrawLine(vRightThighOut.x, vRightThighOut.y, vHipOut.x, vHipOut.y, cColor, kalinlik);
		DrawLine(vLeftCalfOut.x, vLeftCalfOut.y, vLeftThighOut.x, vLeftThighOut.y, cColor, kalinlik);
		DrawLine(vRightCalfOut.x, vRightCalfOut.y, vRightThighOut.x, vRightThighOut.y, cColor, kalinlik);
		DrawLine(vLeftFootOut.x, vLeftFootOut.y, vLeftCalfOut.x, vLeftCalfOut.y, cColor, kalinlik);
		DrawLine(vRightFootOut.x, vRightFootOut.y, vRightCalfOut.x, vRightCalfOut.y, cColor, kalinlik);
	}
}

void DrawCircleFilled(float x, float y, float r, unsigned int p, DWORD color, IDirect3DDevice9* pDevice)
{
	static bool once = false;
	if (!once)  // nvm this,  tried it without once as well 
	{
		pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		once = true;
	}

	vertex* pVertex = new vertex[p + 1];

	for (unsigned int i = 0; i <= p; ++i)
	{
		pVertex[i].x = x + r * cos(D3DX_PI * (i / (p / 2.f)));
		pVertex[i].y = y + r * sin(D3DX_PI * (i / (p / 2.f)));
		pVertex[i].z = 0;
		pVertex[i].rhw = 0;
		pVertex[i].color = color;

	}

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, p - 1, pVertex, sizeof(vertex));

	delete[] pVertex;
}

void Circle(int X, int Y, int radius, int numSides, DWORD Color)
{
	D3DXVECTOR2 Line[128];
	float Step = M_PI * 2.0 / numSides;
	int Count = 0;
	for (float a = 0; a < M_PI * 2.0; a += Step)
	{
		float X1 = radius * cos(a) + X;
		float Y1 = radius * sin(a) + Y;
		float X2 = radius * cos(a + Step) + X;
		float Y2 = radius * sin(a + Step) + Y;
		Line[Count].x = X1;
		Line[Count].y = Y1;
		Line[Count + 1].x = X2;
		Line[Count + 1].y = Y2;
		Count += 2;
	}
	xwwwwwwqq->Begin();
	xwwwwwwqq->Draw(Line, Count, Color);
	xwwwwwwqq->End();
}