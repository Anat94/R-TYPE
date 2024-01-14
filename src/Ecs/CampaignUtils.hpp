/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** CampaignUtils
*/

#ifndef CAMPAIGNUTILS_HPP_
    #define CAMPAIGNUTILS_HPP_
    #include <iostream>
    #include <vector>
    #include "SparseArray.hpp"
    #include "Components.hpp"

class CampaignUtils {
    public:
        static std::vector<std::string> GetAll(sparse_array<component::CampaignMode> &all_campaigns)
        {
            std::vector<std::string> resCampaigns = {};
            for (size_t i = 0; i < all_campaigns.size(); ++i) {
                if (!all_campaigns[i].has_value())
                    continue;
                if (std::find(resCampaigns.begin(), resCampaigns.end(), all_campaigns[i]->_room_name) == resCampaigns.end()) {
                    resCampaigns.push_back(all_campaigns[i]->_room_name);
                }
            }
            return resCampaigns;
        }
};

#endif /* !CAMPAIGNUTILS_HPP_ */
