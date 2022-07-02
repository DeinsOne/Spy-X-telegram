#include <spy/service/functions/DownloadFile/DownloadFile.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/ChatsController/ChatsController.hpp>

void spy::service::functions::DownloadFile::Execute() {
    auto settings = services->GetController<spy::service::controller::SpySettingsController>();

    // Skip file downloading if not enabled media saving
    if (!settings->GetSaveDeletedMedia()) return;

    // Skip if file is larger the the allowed limit
    if (
        file_.size_ > 0 &&
        (file_.size_ / 1024 / 1024) > settings->GetSaveMediaSizeLimit()
    ) return;
    else if (
        file_.remote_->uploaded_size_ > 0 &&
        (file_.remote_->uploaded_size_ / 1024 / 1024) > settings->GetSaveMediaSizeLimit()
    ) return;

    // Download file
    auto filePromise = handler_->Execute<td::td_api::downloadFile>(
        file_.id_,
        priority_,
        0,
        0,
        true
    );

}
