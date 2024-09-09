/**
  ******************************************************************************
  * @file    BSP/Src/touchscreen.c
  * @author  MCD Application Team
  * @brief   This example code shows how to use the touchscreen driver with
  *          a clear button in the top-right corner, color circles moved left,
  *          an eraser button in the bottom-right corner, a drawing canvas,
  *          and Undo/Redo functionality.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>

/** @addtogroup STM32H7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define  CIRCLE_RADIUS        15
#define  CIRCLE_SPACING       10

#define CLEAR_BUTTON_RADIUS   25
#define CLEAR_BUTTON_XPOS     437
#define CLEAR_BUTTON_YPOS     30

#define ERASER_RADIUS         25
#define ERASER_XPOS           437
#define ERASER_YPOS           240

#define UNDO_BUTTON_RADIUS     25
#define UNDO_BUTTON_XPOS       437
#define UNDO_BUTTON_YPOS       100

#define REDO_BUTTON_RADIUS     25
#define REDO_BUTTON_XPOS       437
#define REDO_BUTTON_YPOS       170

#define CANVAS_XPOS           50
#define CANVAS_YPOS           15
#define CANVAS_WIDTH          350
#define CANVAS_HEIGHT         242
#define CANVAS_BORDER_THICKNESS  3

/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       (CIRCLE_RADIUS + CIRCLE_SPACING)
#define  CIRCLE_YPOS(i)       (i * (2 * CIRCLE_RADIUS + CIRCLE_SPACING) + 60)

#define  MAX_HISTORY_SIZE       150

#define NUM_COLORS 5
const uint32_t COLOR_LIST[NUM_COLORS] = {
    UTIL_LCD_COLOR_BLACK,
    UTIL_LCD_COLOR_BLUE,
    UTIL_LCD_COLOR_RED,
    UTIL_LCD_COLOR_YELLOW,
    UTIL_LCD_COLOR_GREEN
};

/* Private Structures and Enumerations ------------------------------------------------------------*/
typedef struct {
    uint16_t x;
    uint16_t y;
    uint32_t color;
} DrawingAction;

/* Global variables ---------------------------------------------------------*/
TS_State_t  TS_State;

/* Private variables ---------------------------------------------------------*/
TS_Init_t hTS;
DrawingAction undoStack[MAX_HISTORY_SIZE];
DrawingAction redoStack[MAX_HISTORY_SIZE];
int undoTop = -1;
int redoTop = -1;

/* Private function prototypes -----------------------------------------------*/
void Touchscreen_DrawBackground_Circles(uint8_t state);
void Touchscreen_DrawButton_Clear(void);
void Touchscreen_DrawButton_Eraser(bool);
void Touchscreen_DrawButton_Undo(void);
void Touchscreen_DrawButton_Redo(void);
void Touchscreen_DrawCanvas(void);
void SaveToUndoStack(uint16_t x, uint16_t y, uint32_t color);
void Undo(void);
void Redo(void);
void RedrawCanvas(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Touchscreen Demo1 : test touchscreen calibration and single touch in polling mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo(void)
{
    uint16_t x1, y1;
    uint32_t ts_status = BSP_ERROR_NONE;
    uint32_t x_size, y_size;
    uint8_t selectedColorIndex = 0; // Black is the default selected color

    BSP_LCD_GetXSize(0, &x_size);
    BSP_LCD_GetYSize(0, &y_size);

    hTS.Width = x_size;
    hTS.Height = y_size;
    hTS.Orientation = TS_SWAP_XY;
    hTS.Accuracy = 5;

    /* Touchscreen initialization */
    ts_status = BSP_TS_Init(0, &hTS);

    if(ts_status == BSP_ERROR_NONE)
    {
        UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);

        // Draw the canvas
        Touchscreen_DrawCanvas();

        // Draw the color palette
        Touchscreen_DrawBackground_Circles(selectedColorIndex);

        // Draw clear, eraser, undo, and redo buttons
        Touchscreen_DrawButton_Clear();
        Touchscreen_DrawButton_Eraser(false);
        Touchscreen_DrawButton_Undo();
        Touchscreen_DrawButton_Redo();

        while (1)
        {
            ts_status = BSP_TS_GetState(0, &TS_State);
            if(TS_State.TouchDetected)
            {
                x1 = TS_State.TouchX;
                y1 = TS_State.TouchY;

                // Check if the touch is on one of the color palette circles
                for (int i = 0; i < NUM_COLORS; i++) {
                    if ((x1 > (CIRCLE_XPOS(i) - CIRCLE_RADIUS)) &&
                        (x1 < (CIRCLE_XPOS(i) + CIRCLE_RADIUS)) &&
                        (y1 > (CIRCLE_YPOS(i) - CIRCLE_RADIUS)) &&
                        (y1 < (CIRCLE_YPOS(i) + CIRCLE_RADIUS))) {
                        selectedColorIndex = i;
                        Touchscreen_DrawBackground_Circles(selectedColorIndex);
                        break;
                    }
                }

                // Check if the touch is on the clear button
                if ((x1 > (CLEAR_BUTTON_XPOS - CLEAR_BUTTON_RADIUS)) &&
                    (x1 < (CLEAR_BUTTON_XPOS + CLEAR_BUTTON_RADIUS)) &&
                    (y1 > (CLEAR_BUTTON_YPOS - CLEAR_BUTTON_RADIUS)) &&
                    (y1 < (CLEAR_BUTTON_YPOS + CLEAR_BUTTON_RADIUS))) {
                    UTIL_LCD_Clear(UTIL_LCD_COLOR_WHITE);
                    Touchscreen_DrawCanvas();
                    Touchscreen_DrawBackground_Circles(selectedColorIndex);
                    Touchscreen_DrawButton_Clear();
                    Touchscreen_DrawButton_Eraser(false);
                    Touchscreen_DrawButton_Undo();
                    Touchscreen_DrawButton_Redo();
                    undoTop = -1; // Clear undo stack on clear
                    redoTop = -1; // Clear redo stack on clear
                }

                // Check if the touch is on the eraser button
                if ((x1 > (ERASER_XPOS - ERASER_RADIUS)) &&
                    (x1 < (ERASER_XPOS + ERASER_RADIUS)) &&
                    (y1 > (ERASER_YPOS - ERASER_RADIUS)) &&
                    (y1 < (ERASER_YPOS + ERASER_RADIUS))) {
                    selectedColorIndex = NUM_COLORS; // Use index outside the color list to denote eraser
                    Touchscreen_DrawBackground_Circles(selectedColorIndex); // Highlight eraser
                }

                // Check if the touch is on the undo button
                if ((x1 > (UNDO_BUTTON_XPOS - UNDO_BUTTON_RADIUS)) &&
                    (x1 < (UNDO_BUTTON_XPOS + UNDO_BUTTON_RADIUS)) &&
                    (y1 > (UNDO_BUTTON_YPOS - UNDO_BUTTON_RADIUS)) &&
                    (y1 < (UNDO_BUTTON_YPOS + UNDO_BUTTON_RADIUS))) {
                    Undo();
                }

                // Check if the touch is on the redo button
                if ((x1 > (REDO_BUTTON_XPOS - REDO_BUTTON_RADIUS)) &&
                    (x1 < (REDO_BUTTON_XPOS + REDO_BUTTON_RADIUS)) &&
                    (y1 > (REDO_BUTTON_YPOS - REDO_BUTTON_RADIUS)) &&
                    (y1 < (REDO_BUTTON_YPOS + REDO_BUTTON_RADIUS))) {
                    Redo();
                }

                // Draw with the selected color or eraser within the canvas
                if (x1 >= CANVAS_XPOS && x1 <= CANVAS_XPOS + CANVAS_WIDTH &&
                    y1 >= CANVAS_YPOS && y1 <= CANVAS_YPOS + CANVAS_HEIGHT) {
                    if (selectedColorIndex < NUM_COLORS) {
                        UTIL_LCD_FillCircle(x1, y1, 5, COLOR_LIST[selectedColorIndex]);
                        SaveToUndoStack(x1, y1, COLOR_LIST[selectedColorIndex]);
                    } else if (selectedColorIndex == NUM_COLORS) {
                        UTIL_LCD_FillCircle(x1, y1, 10, UTIL_LCD_COLOR_WHITE); // Larger dots for eraser
                        SaveToUndoStack(x1, y1, UTIL_LCD_COLOR_WHITE);
                    }
                }
            }
            HAL_Delay(20);
        }
    }
}

/**
  * @brief  Draw Touchscreen Canvas
  * @retval None
  */
void Touchscreen_DrawCanvas(void)
{
    // Draw canvas border
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_BLACK);
    UTIL_LCD_DrawRect(CANVAS_XPOS, CANVAS_YPOS, CANVAS_WIDTH, CANVAS_HEIGHT, UTIL_LCD_COLOR_BLACK);
}

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
void Touchscreen_DrawBackground_Circles(uint8_t state)
{
    for (int i = 0; i < NUM_COLORS; i++) {
        UTIL_LCD_FillCircle(CIRCLE_XPOS(i), CIRCLE_YPOS(i), CIRCLE_RADIUS, COLOR_LIST[i]);
        if (i == state) {
            UTIL_LCD_FillCircle(CIRCLE_XPOS(i), CIRCLE_YPOS(i), CIRCLE_RADIUS - 2, UTIL_LCD_COLOR_WHITE);
        }
    }
    // Redraw eraser button based on state
    if (state == NUM_COLORS) {
         Touchscreen_DrawButton_Eraser(true); // Highlight eraser
    } else {
         Touchscreen_DrawButton_Eraser(false); // Normal state
    }

}

/**
  * @brief  Draw Clear Button
  * @retval None
  */
void Touchscreen_DrawButton_Clear(void)
{
    UTIL_LCD_FillCircle(CLEAR_BUTTON_XPOS, CLEAR_BUTTON_YPOS, CLEAR_BUTTON_RADIUS, UTIL_LCD_COLOR_RED);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_RED);
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(5000, CLEAR_BUTTON_YPOS - 13, (uint8_t *)"Clear", CENTER_MODE);
}

/**
  * @brief  Draw Eraser Button
  * @retval None
  */
void Touchscreen_DrawButton_Eraser(bool selected)
{
	UTIL_LCD_DrawCircle(ERASER_XPOS, ERASER_YPOS, ERASER_RADIUS, UTIL_LCD_COLOR_GRAY);
	if (selected) {
	        // Draw eraser with no filling
			UTIL_LCD_FillCircle(ERASER_XPOS, ERASER_YPOS, ERASER_RADIUS-3, UTIL_LCD_COLOR_WHITE);
			UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_WHITE);
	        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_GRAY);
	    } else {
	        // Draw eraser with filling
	        UTIL_LCD_FillCircle(ERASER_XPOS, ERASER_YPOS, ERASER_RADIUS, UTIL_LCD_COLOR_GRAY);
	        UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_GRAY);
	        UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
	    }

    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(5000, ERASER_YPOS - 13, (uint8_t *)"Eraser", CENTER_MODE);
}

/**
  * @brief  Draw Undo Button
  * @retval None
  */
void Touchscreen_DrawButton_Undo(void)
{
    UTIL_LCD_FillCircle(UNDO_BUTTON_XPOS, UNDO_BUTTON_YPOS, UNDO_BUTTON_RADIUS, UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLUE);
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(5000, UNDO_BUTTON_YPOS - 13, (uint8_t *)"Undo", CENTER_MODE);
}

/**
  * @brief  Draw Redo Button
  * @retval None
  */
void Touchscreen_DrawButton_Redo(void)
{
    UTIL_LCD_FillCircle(REDO_BUTTON_XPOS, REDO_BUTTON_YPOS, REDO_BUTTON_RADIUS, UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
    UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_GREEN);
    UTIL_LCD_SetFont(&Font12);
    UTIL_LCD_DisplayStringAt(5000, REDO_BUTTON_YPOS - 13, (uint8_t *)"Redo", CENTER_MODE);
}

/**
  * @brief  Save Drawing Action to Undo Stack
  * @param  x     : x coordinate
  * @param  y     : y coordinate
  * @param  color : color of the drawing
  * @retval None
  */
void SaveToUndoStack(uint16_t x, uint16_t y, uint32_t color)
{
    if (undoTop < MAX_HISTORY_SIZE - 1) {
        undoStack[++undoTop].x = x;
        undoStack[undoTop].y = y;
        undoStack[undoTop].color = color;
        redoTop = -1; // Clear redo stack on new action
    }
}

/**
  * @brief  Perform Undo Operation
  * @retval None
  */
void Undo(void)
{
    if (undoTop >= 0) {
        DrawingAction action = undoStack[undoTop--];
        // Clear the drawing at the action position
        UTIL_LCD_FillCircle(action.x, action.y, 10, UTIL_LCD_COLOR_WHITE); // Larger size for undo
        // Push the action to redo stack
        if (redoTop < MAX_HISTORY_SIZE - 1) {
            redoStack[++redoTop] = action;
        }
        // Redraw the canvas and history
        Touchscreen_DrawCanvas();
        RedrawCanvas();
    }
}

/**
  * @brief  Perform Redo Operation
  * @retval None
  */
void Redo(void)
{
    if (redoTop >= 0) {
        DrawingAction action = redoStack[redoTop--];
        // Draw the action on the canvas
        UTIL_LCD_FillCircle(action.x, action.y, 5, action.color);
        // Push the action to undo stack
        if (undoTop < MAX_HISTORY_SIZE - 1) {
            undoStack[++undoTop] = action;
        }
    }
}

/**
  * @brief  Redraw Canvas with History
  * @retval None
  */
void RedrawCanvas(void)
{
    // Redraw the entire canvas by scanning through the history
    for (int i = 0; i <= undoTop; i++) {
        UTIL_LCD_FillCircle(undoStack[i].x, undoStack[i].y, 5, undoStack[i].color);
    }
}

/* End of file ----------------------------------------------------------------*/
