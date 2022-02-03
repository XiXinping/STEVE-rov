import asyncio


async def camera_server():
    while True:
        print("*camera noise*")
        await asyncio.sleep(1)


async def main_server():
    while True:
        print("*main server noise*")
        await asyncio.sleep(1/6)


def main():
    loop = asyncio.get_event_loop()
    asyncio.ensure_future(camera_server())
    asyncio.ensure_future(main_server())
    loop.run_forever()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print('')
