/**
 * @file Interface.h
 * @brief Файл, содержащий описание интерфейсов
 */

#pragma once

/**
 * @brief Интерфейс с методом обновления данных
 */
class UpdatableDataInterface
{
public:

    /**
     * @brief Метод обновления данных
     * @param msec Количество миллисекунд, прошедшее с момента последнего обновления данных
     */
    virtual void updateData(int msec) = 0;
};

/**
 * @brief Интерфейс с методом обновления изображения
 */
class UpdatableImageInterface
{
public:

    /**
     * @brief Метод обновления изображения
     */
    virtual void updateImage() = 0;
};